#!/usr/bin/env python3
"""Create a GP4 project for PkgTool.Core from the files being packaged."""
import argparse
import datetime
from xml.sax.saxutils import quoteattr


def directory_tree(paths):
    root = {}
    for path in paths:
        node = root
        for part in path.split("/")[:-1]:
            node = node.setdefault(part, {})
    return root


def emit_directories(node, depth, output):
    for name in sorted(node):
        indent = "\t" * depth
        if node[name]:
            output.append("%s<dir targ_name=%s>" % (indent, quoteattr(name)))
            emit_directories(node[name], depth + 1, output)
            output.append("%s</dir>" % indent)
        else:
            output.append("%s<dir targ_name=%s />" % (indent, quoteattr(name)))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", required=True)
    parser.add_argument("--content-id", required=True)
    parser.add_argument("files", nargs="+")
    args = parser.parse_args()

    files = [name.replace("\\", "/") for name in args.files]
    timestamp = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
    xml = [
        '<?xml version="1.0"?>',
        '<psproject xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" fmt="gp4" version="1000">',
        "\t<volume>",
        "\t\t<volume_type>pkg_ps4_app</volume_type>",
        "\t\t<volume_id>PS4VOLUME</volume_id>",
        "\t\t<volume_ts>%s</volume_ts>" % timestamp,
        '\t\t<package content_id=%s passcode="00000000000000000000000000000000" storage_type="digital50" app_type="full" />' % quoteattr(args.content_id),
        '\t\t<chunk_info chunk_count="1" scenario_count="1">',
        "\t\t\t<chunks>",
        '\t\t\t\t<chunk id="0" layer_no="0" label="Chunk #0" />',
        "\t\t\t</chunks>",
        '\t\t\t<scenarios default_id="0">',
        '\t\t\t\t<scenario id="0" type="sp" initial_chunk_count="1" label="Scenario #0">0</scenario>',
        "\t\t\t</scenarios>",
        "\t\t</chunk_info>",
        "\t</volume>",
        '\t<files img_no="0">',
    ]
    for name in files:
        xml.append("\t\t<file targ_path=%s orig_path=%s />" % (quoteattr(name), quoteattr(name)))
    xml.extend(["\t</files>", "\t<rootdir>"])
    emit_directories(directory_tree(files), 2, xml)
    xml.extend(["\t</rootdir>", "</psproject>"])

    with open(args.out, "w", encoding="utf-8", newline="\n") as output:
        output.write("\n".join(xml) + "\n")


if __name__ == "__main__":
    main()
