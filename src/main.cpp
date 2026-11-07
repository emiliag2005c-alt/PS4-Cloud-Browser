#include <orbis/libkernel.h>
#include <orbis/VideoOut.h>
#include <orbis/Keyboard.h>
#include <orbis/UserService.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// v0.4 diagnostic renderer. The framebuffer setup follows the OpenOrbis
// samples/_common/graphics.cpp and samples/graphics/graphics/main.cpp pattern.
// Diagnostic log: /data/PS4CloudBrowser.log

#define SCREEN_W 1920
#define SCREEN_H 1080
#define PIXEL_DEPTH 4
#define FRAMEBUFFER_COUNT 2
#define DIRECT_MEMORY_SIZE 0x2000000
#define DIRECT_MEMORY_ALIGNMENT 0x200000

struct Color { uint8_t r, g, b; };

static FILE *gLog = nullptr;
static int gKeyboardHandle = -1;
static bool gKeyboardReady = false;
static char gSearchText[64] = "";
static int gKeyCount = 0;
static int gLastKey = 0;

static void Log(const char *message, int value = 0) {
    if (gLog) {
        fprintf(gLog, "%s: 0x%08X\n", message, (unsigned int)value);
        fflush(gLog);
    }
    printf("[PS4 Cloud Browser] %s: 0x%08X\n", message, (unsigned int)value);
}

class Scene2D {
    int width, height, video;
    size_t frameBufferSize, allocationSize;
    off_t directMemOff;
    uintptr_t videoMemSP;
    void *videoMem;
    char *buffers[FRAMEBUFFER_COUNT];
    OrbisKernelEqueue flipQueue;
    OrbisVideoOutBufferAttribute attr;
    int active;

public:
    Scene2D(int w, int h)
        : width(w), height(h), video(-1),
          frameBufferSize((size_t)w * h * PIXEL_DEPTH),
          allocationSize(0), directMemOff(0), videoMemSP(0),
          videoMem(nullptr), flipQueue(0), active(0) {
        buffers[0] = nullptr;
        buffers[1] = nullptr;
        memset(&attr, 0, sizeof(attr));
    }

    bool Init() {
        int rc = sceVideoOutOpen(ORBIS_VIDEO_USER_MAIN, ORBIS_VIDEO_OUT_BUS_MAIN, 0, 0);
        if (rc < 0) { Log("VIDEOOUT_OPEN_FAILED", rc); return false; }
        video = rc;
        Log("VIDEOOUT_OPEN_OK", video);

        rc = sceKernelCreateEqueue(&flipQueue, "PS4 Cloud Browser flip queue");
        if (rc < 0) { Log("CREATE_EQUEUE_FAILED", rc); return false; }
        Log("CREATE_EQUEUE_OK", rc);

        rc = sceVideoOutAddFlipEvent(flipQueue, video, 0);
        if (rc < 0) { Log("ADD_FLIP_EVENT_FAILED", rc); return false; }
        Log("ADD_FLIP_EVENT_OK", rc);

        allocationSize = (DIRECT_MEMORY_SIZE + DIRECT_MEMORY_ALIGNMENT - 1)
                       / DIRECT_MEMORY_ALIGNMENT * DIRECT_MEMORY_ALIGNMENT;
        rc = sceKernelAllocateDirectMemory(
            0, sceKernelGetDirectMemorySize(), allocationSize,
            DIRECT_MEMORY_ALIGNMENT, 3, &directMemOff);
        if (rc < 0) { Log("ALLOCATE_DIRECT_MEMORY_FAILED", rc); return false; }
        Log("ALLOCATE_DIRECT_MEMORY_OK", rc);

        rc = sceKernelMapDirectMemory(&videoMem, allocationSize, 0x33, 0,
                                      directMemOff, DIRECT_MEMORY_ALIGNMENT);
        if (rc < 0) {
            Log("MAP_DIRECT_MEMORY_FAILED", rc);
            sceKernelReleaseDirectMemory(directMemOff, allocationSize);
            return false;
        }
        Log("MAP_DIRECT_MEMORY_OK", rc);

        videoMemSP = reinterpret_cast<uintptr_t>(videoMem);
        for (int i = 0; i < FRAMEBUFFER_COUNT; ++i) {
            buffers[i] = reinterpret_cast<char *>(videoMemSP);
            videoMemSP += frameBufferSize;
        }

        // Same SRGB/pitch setup used by the OpenOrbis 2D graphics sample.
        sceVideoOutSetBufferAttribute(&attr, 0x80000000, 1, 0,
                                      width, height, width);
        rc = sceVideoOutRegisterBuffers(video, 0,
                 reinterpret_cast<void **>(buffers), FRAMEBUFFER_COUNT, &attr);
        if (rc < 0) { Log("REGISTER_BUFFERS_FAILED", rc); return false; }
        Log("REGISTER_BUFFERS_OK", rc);

        rc = sceVideoOutSetFlipRate(video, 0);
        if (rc < 0) { Log("SET_FLIP_RATE_FAILED", rc); return false; }
        Log("SET_FLIP_RATE_OK", rc);
        return true;
    }

    void Fill(Color c) { Rect(0, 0, width, height, c); }

    void Pixel(int x, int y, Color c) {
        if (x < 0 || y < 0 || x >= width || y >= height || !buffers[active]) return;
        uint32_t value = 0x80000000u | ((uint32_t)c.r << 16) |
                         ((uint32_t)c.g << 8) | (uint32_t)c.b;
        reinterpret_cast<uint32_t *>(buffers[active])[(size_t)y * width + x] = value;
    }

    void Rect(int x, int y, int w, int h, Color c) {
        if (w <= 0 || h <= 0) return;
        int x0 = x < 0 ? 0 : x, y0 = y < 0 ? 0 : y;
        int x1 = x + w > width ? width : x + w;
        int y1 = y + h > height ? height : y + h;
        for (int py = y0; py < y1; ++py)
            for (int px = x0; px < x1; ++px) Pixel(px, py, c);
    }

    bool Flip(int id) {
        int rc = sceVideoOutSubmitFlip(video, active, ORBIS_VIDEO_OUT_FLIP_VSYNC, id);
        if (rc < 0) { Log("SUBMIT_FLIP_FAILED", rc); return false; }

        // Follow the OpenOrbis sample: wait for this frame's flip event.
        OrbisKernelEvent event;
        int count = 0;
        for (int attempt = 0; attempt < 120; ++attempt) {
            OrbisVideoOutFlipStatus status;
            rc = sceVideoOutGetFlipStatus(video, &status);
            if (rc == 0 && status.flipArg == id) {
                Log("FRAME_FLIP_CONFIRMED", id);
                return true;
            }
            rc = sceKernelWaitEqueue(flipQueue, &event, 1, &count, 0);
            if (rc != 0) {
                Log("WAIT_FLIP_EVENT_FAILED", rc);
                break;
            }
        }
        Log("FRAME_FLIP_NOT_CONFIRMED", id);
        return false;
    }

    void Swap() { active = (active + 1) % FRAMEBUFFER_COUNT; }
};

static const uint8_t FONT[36][7] = {
    {14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},
    {30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
    {14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{14,4,4,4,4,4,14},
    {7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
    {17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},
    {30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
    {15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
    {17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
    {17,17,10,4,4,4,4},{31,1,2,4,8,16,31},{14,17,19,21,25,17,14},
    {4,12,4,4,4,4,14},{14,17,1,2,4,8,31},{30,1,1,14,1,1,30},
    {2,6,10,18,31,2,2},{31,16,16,30,1,1,30},{14,16,16,30,17,17,14},
    {31,1,2,4,8,8,8},{14,17,17,14,17,17,14},{14,17,17,15,1,1,14}
};

static void Text(Scene2D &s, int x, int y, const char *str, int scale, Color color) {
    int cursor = x;
    for (const char *p = str; *p; ++p) {
        if (*p == ' ') { cursor += 4 * scale; continue; }
        int index = (*p >= 'A' && *p <= 'Z') ? *p - 'A' :
                    ((*p >= '0' && *p <= '9') ? 26 + *p - '0' : -1);
        if (index >= 0) {
            for (int row = 0; row < 7; ++row)
                for (int col = 0; col < 5; ++col)
                    if (FONT[index][row] & (1 << (4 - col)))
                        s.Rect(cursor + col * scale, y + row * scale,
                               scale, scale, color);
        }
        cursor += 6 * scale;
    }
}


static char KeyToChar(int key) {
    if (key >= 4 && key <= 29) return (char)('A' + key - 4);
    if (key >= 30 && key <= 38) return (char)('1' + key - 30);
    if (key == 39) return '0';
    if (key == ORBIS_KEYBOARD_KEY_BACKSPACE) return '\\b';
    if (key == ORBIS_KEYBOARD_KEY_RETURN) return '\\n';
    if (key == 44) return ' ';
    return 0;
}
static void PollKeyboard() {
    if (!gKeyboardReady || gKeyboardHandle < 0) return;
    OrbisKeyboardData data;
    memset(&data, 0, sizeof(data));
    int rc = sceKeyboardReadState(gKeyboardHandle, &data);
    if (rc < 0) return;
    for (int i = 0; i < data.nkeys && i < 32; ++i) {
        int key = data.keycodes[i];
        gLastKey = key;
        char c = KeyToChar(key);
        if (c == '\\b') { size_t n = strlen(gSearchText); if (n) gSearchText[n - 1] = 0; }
        else if (c && c != '\\n' && strlen(gSearchText) < sizeof(gSearchText) - 1) {
            size_t n = strlen(gSearchText); gSearchText[n] = c; gSearchText[n + 1] = 0;
        }
        ++gKeyCount;
    }
}
static void DrawKeyboardTest(Scene2D &s) {
    const Color panel={20,29,47}, panel2={25,38,59};
    const Color white={235,242,255}, muted={135,155,180};
    const Color cyan={45,205,220}, green={70,220,155}, orange={245,175,75};
    s.Rect(140,640,1640,300,panel); s.Rect(140,640,1640,7,cyan);
    Text(s,180,680,"INPUT LAB KEYBOARD AND MOUSE",3,white);
    Text(s,180,740,gKeyboardReady ? "KEYBOARD CONNECTED" : "KEYBOARD NOT DETECTED",2,gKeyboardReady ? green : orange);
    Text(s,850,740,"KEY EVENTS",2,muted);
    char number[24]; snprintf(number,sizeof(number),"%d",gKeyCount); Text(s,1130,740,number,2,white);
    Text(s,180,795,"SEARCH TEST TYPE HERE",2,muted);
    s.Rect(180,835,940,62,panel2); Text(s,200,852,gSearchText,2,white);
    Text(s,1260,795,"LAST KEY CODE",2,muted);
    snprintf(number,sizeof(number),"%d",gLastKey); Text(s,1260,850,number,3,cyan);
    Text(s,180,920,"MOUSE LEFT RIGHT CLICK TEST PENDING API RESEARCH",2,orange);
}

static void DrawInterface(Scene2D &s) {
    const Color bg={11,16,29}, panel={20,29,47}, panel2={25,38,59};
    const Color white={235,242,255}, muted={135,155,180};
    const Color cyan={45,205,220}, green={70,220,155}, orange={245,175,75};

    s.Fill(bg);
    s.Rect(0,0,SCREEN_W,12,cyan);
    s.Rect(100,92,8,64,cyan);
    Text(s,132,108,"PS4 CLOUD BROWSER",3,white);
    Text(s,1450,116,"BUILD 04",2,cyan);
    Text(s,140,330,"PS4 CLOUD BROWSER",8,white);
    s.Rect(145,420,850,8,cyan);
    Text(s,150,475,"DIAGNOSTICO DE VIDEO",3,muted);
    s.Rect(140,640,760,150,panel); s.Rect(140,640,760,7,green);
    Text(s,185,685,"VIDEOOUT ACTIVO",4,green); Text(s,185,745,"FRAMEBUFFER REGISTRADO",2,white);
    s.Rect(940,640,840,150,panel2); s.Rect(940,640,840,7,orange);
    Text(s,985,685,"MOTOR WEB EN DESARROLLO",3,orange); Text(s,985,745,"GEFORCE NOW NO CONECTADO",2,white);
    DrawKeyboardTest(s); s.Rect(140,970,1640,2,panel2);
    Text(s,145,990,"VERSION 05 - PRUEBA DE ENTRADAS",2,muted);
    Text(s,1510,970,"OPENORBIS",2,cyan);
}

int main(void) {
    gLog = fopen("/data/PS4CloudBrowser.log", "w");
    Log("APP_START", 4);

    Scene2D scene(SCREEN_W, SCREEN_H);
    if (!scene.Init()) {
        Log("SCENE_INIT_FAILED", 1);
        if (gLog) fclose(gLog);
        for (;;) sceKernelUsleep(1000000);
    }

    Log("SCENE_INIT_COMPLETE", 0);
    int userId = -1;
    int userRc = sceUserServiceInitialize(nullptr); Log("USER_SERVICE_INIT", userRc);
    userRc = sceUserServiceGetInitialUser(&userId); Log("INITIAL_USER", userRc);
    int keyboardRc = sceKeyboardInit(); Log("KEYBOARD_INIT", keyboardRc);
    if (keyboardRc >= 0 && userRc >= 0) {
        gKeyboardHandle = sceKeyboardOpen(userId, 0, 0, nullptr);
        Log("KEYBOARD_OPEN", gKeyboardHandle); gKeyboardReady = gKeyboardHandle >= 0;
    }
    DrawInterface(scene);
    if (!scene.Flip(0)) {
        Log("FIRST_FRAME_FAILED", 0);
        if (gLog) fclose(gLog);
        for (;;) sceKernelUsleep(1000000);
    }

    scene.Swap();
    DrawInterface(scene);
    if (!scene.Flip(1)) {
        Log("SECOND_FRAME_FAILED", 1);
        if (gLog) fclose(gLog);
        for (;;) sceKernelUsleep(1000000);
    }

    Log("GRAPHICS_TEST_PASSED", 0);
    int frameId = 2;
    for (;;) {
        PollKeyboard(); scene.Swap(); DrawInterface(scene);
        if (!scene.Flip(frameId++)) sceKernelUsleep(100000);
        sceKernelUsleep(16000);
    }
    return 0;
}
