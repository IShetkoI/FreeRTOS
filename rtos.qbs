import qbs
import qbs.FileInfo
import qbs.ModUtils

CppApplication
{
    files: [
        "Core/Inc/freertos.h",
    ]
    name: "RTOS"
    type: [
        "application",
        "bin",
        "hex",
    ]


    Depends
    {
        name: "cpp"
    }


    consoleApplication:          true
    cpp.positionIndependentCode: false
    cpp.executableSuffix:        ".elf"
    input.baseDir:               path


    property string Home: path

    property string Inc:         Home + "/Core/Inc"
    property string Src:         Home + "/Core/Src"
    property string Startup:     Home + "/Core/Startup"

    property string CMSIS:       Home + "/Drivers/CMSIS"
    property string HAL:         Home + "/Drivers/STM32F7xx_HAL_Driver"

    property string Adc:         Home + "/Analog/Adc"
    property string Bmp280:      Home + "/Sensors/Bmp280"
    property string Clock:       Home + "/SystemCore/Clock"
    property string Dac:         Home + "/Analog/Dac"
    property string Dma:         Home + "/SystemCore/Dma"
    property string Gpio:        Home + "/SystemCore/Gpio"
    property string Spi:         Home + "/Connectivity/Spi"
    property string FreeRTOS:    Home + "/Middleware/FreeRTOS/Src"
    property string Timer:       Home + "/Timers/Timer"
    property string Usart:       Home + "/Connectivity/Usart"


    Group
    {
        name: "Inc"
        files:
        [
            Inc + "/*.h",
        ]
    }

    Group
    {
        name: "Src"
        files:
        [
            Src + "/*.c",
            Src + "/*.cpp",
        ]
    }

    Group
    {
        name: "Startup"
        files:
        [
            Startup + "/*.s",
        ]
    }

    Group
    {
        name: "CMSIS"
        files:
        [
            CMSIS + "/Include/*.h",
            CMSIS + "/Device/ST/STM32F7xx/Include/*.h",
        ]
    }

    Group
    {
        name: "HAL"
        files:
        [
            HAL + "/Inc/*.h",
            HAL + "/Src/*.c",
            HAL + "/Inc/Legacy/*.h",
        ]
    }

    Group
    {
        name: "Adc"
        files:
        [
            Adc + "/Inc/*.h",
            Adc + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Bmp280"
        files:
        [
            Bmp280 + "/Inc/*.h",
            Bmp280 + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Clock"
        files:
        [
            Clock + "/Inc/*.h",
            Clock + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Dac"
        files:
        [
            Dac + "/Inc/*.h",
            Dac + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Dma"
        files:
        [
            Dma + "/Inc/*.h",
            Dma + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Gpio"
        files:
        [
            Gpio + "/Inc/*.h",
            Gpio + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Spi"
        files:
        [
            Spi + "/Inc/*.h",
            Spi + "/Src/*.c",
        ]
    }

    Group
    {
        name: "FreeRTOS"
        files:
        [
            FreeRTOS + "/Inc/*.h",
            FreeRTOS + "/CMSIS_RTOS_V2/*.h",
            FreeRTOS + "/CMSIS_RTOS_V2/*.c",
            FreeRTOS + "/portable/GCC/ARM_CM7/r0p1/*.c",
            FreeRTOS + "/portable/GCC/ARM_CM7/r0p1/*.h",
            FreeRTOS + "/portable/MemMang/*.c",
            FreeRTOS + "/*.c",
        ]
    }

    Group
    {
        name: "Timer"
        files:
        [
            Timer + "/Inc/*.h",
            Timer + "/Src/*.c",
        ]
    }

    Group
    {
        name: "Usart"
        files:
        [
            Usart + "/Inc/*.h",
            Usart + "/Src/*.c",
        ]
    }

    Group
    {
        name: "LD"
        files:
        [
            Home + "/*.ld",
        ]
    }


    cpp.includePaths: [
        Inc,

        CMSIS + "/Include",
        CMSIS + "/Device/ST/STM32F7xx/Include",

        HAL + "/Inc",
        HAL + "/Inc/Legacy",

        Adc         + "/Inc",
        Bmp280      + "/Inc",
        Clock       + "/Inc",
        Dac         + "/Inc",
        Dma         + "/Inc",
        Gpio        + "/Inc",
        Spi         + "/Inc",
        FreeRTOS + "/Inc",
        FreeRTOS + "/CMSIS_RTOS_V2",
        FreeRTOS + "/portable/GCC/ARM_CM7/r0p1/",
        Timer       + "/Inc",
        Usart       + "/Inc",
    ]

    cpp.defines: [
        "USE_HAL_DRIVER",
        "STM32F7xx",
        "USE_DSTM32F767xx",
        "__weak=__attribute__((weak))",
        "__packed=__attribute__((__packed__))",
    ]

    cpp.commonCompilerFlags: [
        "-mcpu=cortex-m7",
        "-mfloat-abi=hard",
        "-mfpu=fpv5-d16",
        "-mthumb",
    ]

    cpp.driverFlags: [
        "-mcpu=cortex-m7",
        "-mfloat-abi=hard",
        "-mfpu=fpv5-d16",
        "-mthumb",
        "-Xlinker",
        "--gc-sections",
        "-specs=nosys.specs",
        "-specs=nano.specs",
        "-Wl,-Map=" + path + "/../QT-STM32746G-Discovery.map",
    ]

    cpp.linkerFlags: [
        "--start-group",
        "-T" + path + "/STM32F767ZITX_FLASH.ld",
        "-u_printf_float",
        "--end-group",
    ]

    Properties {
        condition: qbs.buildVariant === "debug"
        cpp.debugInformation: true
        cpp.optimization: "none"
    }

    Properties {
        condition: cpp.debugInformation
        cpp.defines: outer.concat("DEBUG")
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.staticLibraries: [
        ":libm.a",
    ]

    Rule {
        id: binDebugFrmw
        condition: qbs.buildVariant === "debug"
        inputs: ["application"]

        Artifact {
            fileTags: ["bin"]
            filePath: input.baseDir + "/" + input.baseName + ".bin"
        }

        prepare: {
            var objCopyPath = "/home/AllCash/Documents/Bare/gcc/bin/arm-none-eabi-objcopy"
            var argsConv = ["-O", "binary", input.filePath, output.filePath]
            var cmd = new Command(objCopyPath, argsConv)
            cmd.description = "converting to BIN: " + FileInfo.fileName(
                        input.filePath) + " -> " + input.baseName + ".bin"

            var argsFlashingInternalFlash =
            [
                        "-f", "/usr/share/openocd/scripts/board/st_nucleo_f7.cfg",
                        "-c", "init",
                        "-c", "reset init",
                        "-c", "flash write_image erase " + input.filePath,
                        "-c", "reset",
                        "-c", "shutdown"
            ]

            var cmdFlashInternalFlash = new Command("openocd", argsFlashingInternalFlash);
            cmdFlashInternalFlash.description = "Wrtie to the internal flash"
            return [cmd,cmdFlashInternalFlash]
        }
    }
}
