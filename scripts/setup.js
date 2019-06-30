const fs = require("fs");

const Util = require("./Util");
const { LIB_PATH, WINDOWS_LIB_PATH } = require("./Constants");

const WIN_LIB_INFO = [
    {
        extractPath: `${WINDOWS_LIB_PATH}/SDL2`,
        innerFolder: "SDL2-2.0.9",
        url: "https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip",
        hash: "ea266ef613f88433f493498f9e72e6bed5d03e4f3fde5b571a557a754ade9065",
    },
    {
        extractPath: `${WINDOWS_LIB_PATH}/SDL2_image`,
        innerFolder: "SDL2_image-2.0.4",
        url: "https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.4-VC.zip",
        hash: "4e15fad9de43d738b476e422eef1910432443cead60d2084b3ef01d3f4a76087",
    },
    {
        extractPath: `${WINDOWS_LIB_PATH}/SDL2_ttf`,
        innerFolder: "SDL2_ttf-2.0.14",
        url: "https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.14-VC.zip",
        hash: "f8ed51e7bb1122cf4dbbc4c47ab8eb13614fae233a20a6b9a5694769b0413c1a",
        exclude: ["zlib1.dll"],
    },
];

const GTEST_LIB_INFO = {
    extractPath: `${LIB_PATH}/gtest`,
    innerFolder: "googletest-release-1.8.1",
    url: "https://github.com/google/googletest/archive/release-1.8.1.zip",
    hash: "927827c183d01734cc5cfef85e0ff3f5a92ffe6188e0d18e909c5efebf28a0c7",
};

function setup() {
    const platform = os.platform();
    try {
        switch(platform) {

            case "win32":
                setupWindows();
                break;

            case "linux":
                setupLinux();
                break;

            case "darwin":
                setupOSX();
                break;

            default:
                Util.error(`Unknown platform detected: ${platform}`)
        }
    } finally {
        Util.cleanup();
    }
}

function setupWindows() {
    Util.log("Downloading SDL2 binaries");
    WIN_LIB_INFO.forEach(Util.downloadAndUnzip);
}

function setupLinux() {
    Util.exec([
        'sudo',
        'apt-get',
        'install',
        'libsdl2-2.0',
        'libsdl2-dev',
        'libsdl2-image-2.0-0',
        'libsdl2-image-dev',
        'libsdl2-ttf-2.0-0',
        'libsdl2-ttf-dev',
    ]);
}

function setupOSX() {
    Util.error("OSX setup not implemented.");
}

function downloadGTestIfNeeded() {
    if (fs.existsSync(`${LIB_PATH}/gtest`)) {
        return;
    }

    Util.downloadAndUnzip(GTEST_LIB_INFO);
}

if (require.main === module) {
    setup();
}
