import platform
import common

def setup():

    os_name = platform.system()
    if os_name == "Windows":
        import win_build
        win_build.build()
    elif os_name == "Linux":
        import linux_build
        linux_build.build()

    common.cleanup()

if __name__ == "__main__":
    setup()