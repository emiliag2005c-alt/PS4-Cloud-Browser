#include <orbis/libkernel.h>
#include <orbis/VideoOut.h>
#include <stdint.h>
#include <stddef.h>

#define SCREEN_W 1920
#define SCREEN_H 1080

struct Color { uint8_t r, g, b; };

class Scene2D {
    int width, height, video, frameBufferCount, active;
    size_t bufferSize, allocationSize;
    off_t directMemOff;
    uintptr_t videoMemSP;
    void *videoMem;
    char **buffers;
    OrbisKernelEqueue flipQueue;
    OrbisVideoOutBufferAttribute attr;

public:
    Scene2D(int w, int h)
        : width(w), height(h), video(-1), frameBufferCount(0), active(0),
          bufferSize((size_t)w * h * 4), allocationSize(0), directMemOff(0),
          videoMemSP(0), videoMem(nullptr), buffers(nullptr), flipQueue(0) {}

    bool Init(size_t memSize, int count) {
        video = sceVideoOutOpen(ORBIS_VIDEO_USER_MAIN, ORBIS_VIDEO_OUT_BUS_MAIN, 0, 0);
        if (video < 0) return false;
        if (sceKernelCreateEqueue(&flipQueue, "PS4 Cloud Browser flip queue") < 0) return false;
        if (sceVideoOutAddFlipEvent(flipQueue, video, 0) < 0) return false;

        const int alignment = 0x200000;
        allocationSize = (memSize + alignment - 1) / alignment * alignment;
        if (sceKernelAllocateDirectMemory(0, sceKernelGetDirectMemorySize(),
                allocationSize, alignment, 3, &directMemOff) < 0) return false;
        if (sceKernelMapDirectMemory(&videoMem, allocationSize, 0x33, 0,
                directMemOff, alignment) < 0) {
            sceKernelReleaseDirectMemory(directMemOff, allocationSize);
            return false;
        }

        videoMemSP = reinterpret_cast<uintptr_t>(videoMem);
        buffers = new char*[count];
        if (!buffers) return false;
        frameBufferCount = count;
        for (int i = 0; i < count; ++i) {
            buffers[i] = reinterpret_cast<char*>(videoMemSP);
            videoMemSP += bufferSize;
        }

        sceVideoOutSetBufferAttribute(&attr, 0x80000000, 1, 0,
                                      width, height, width);
        if (sceVideoOutRegisterBuffers(video, 0,
                reinterpret_cast<void**>(buffers), count, &attr) < 0) return false;
        sceVideoOutSetFlipRate(video, 0);
        return true;
    }

    void Fill(Color c) { Rect(0, 0, width, height, c); }

    void Pixel(int x, int y, Color c) {
        if (x < 0 || y < 0 || x >= width || y >= height) return;
        uint32_t value = 0x80000000u | ((uint32_t)c.r << 16) |
                         ((uint32_t)c.g << 8) | c.b;
        reinterpret_cast<uint32_t*>(buffers[active])[y * width + x] = value;
    }

    void Rect(int x, int y, int w, int h, Color c) {
        if (w <= 0 || h <= 0) return;
        int x0 = x < 0 ? 0 : x, y0 = y < 0 ? 0 : y;
        int x1 = x + w > width ? width : x + w;
        int y1 = y + h > height ? height : y + h;
        for (int py = y0; py < y1; ++py)
            for (int px = x0; px < x1; ++px) Pixel(px, py, c);
    }

    void Flip(int id) {
        sceVideoOutSubmitFlip(video, active, ORBIS_VIDEO_OUT_FLIP_VSYNC, id);
        OrbisKernelEvent event;
        int count = 0;
        for (;;) {
            OrbisVideoOutFlipStatus status;
            if (sceVideoOutGetFlipStatus(video, &status) == 0 && status.flipArg == id) break;
            if (sceKernelWaitEqueue(flipQueue, &event, 1, &count, 0) != 0) break;
        }
    }

    void Swap() { active = (active + 1) % frameBufferCount; }
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

static void DrawInterface(Scene2D &s) {
    const Color bg={11,16,29}, panel={20,29,47}, panel2={25,38,59};
    const Color white={235,242,255}, muted={135,155,180};
    const Color cyan={45,205,220}, green={70,220,155}, orange={245,175,75};

    s.Fill(bg);
    s.Rect(0,0,SCREEN_W,12,cyan);
    s.Rect(100,92,8,64,cyan);
    Text(s,132,108,"PS4 CLOUD BROWSER",3,white);
    Text(s,1450,116,"BUILD 03",2,cyan);

    Text(s,140,330,"PS4 CLOUD BROWSER",8,white);
    s.Rect(145,420,850,8,cyan);
    Text(s,150,475,"NAVEGADOR EXPERIMENTAL",3,muted);

    s.Rect(140,640,760,190,panel);
    s.Rect(140,640,760,7,green);
    Text(s,185,690,"APLICACION ACTIVA",4,green);
    Text(s,185,765,"SISTEMA DE VIDEO INICIALIZADO",2,white);

    s.Rect(940,640,840,190,panel2);
    s.Rect(940,640,840,7,orange);
    Text(s,985,690,"MOTOR WEB EN DESARROLLO",3,orange);
    Text(s,985,765,"GEFORCE NOW NO CONECTADO",2,white);

    s.Rect(140,930,1640,2,panel2);
    Text(s,145,970,"VERSION 03 PRIMERA INTERFAZ GRAFICA",2,muted);
    Text(s,1510,970,"OPENORBIS",2,cyan);
}

int main(void) {
    Scene2D scene(SCREEN_W, SCREEN_H);
    if (!scene.Init(0x2000000, 2)) {
        for (;;) sceKernelUsleep(1000000);
    }

    int frame = 0;
    DrawInterface(scene);
    scene.Flip(frame++);
    scene.Swap();
    DrawInterface(scene);
    scene.Flip(frame);

    // Static UI prototype. Controller navigation will be added in a later build.
    for (;;) sceKernelUsleep(1000000);
    return 0;
}
