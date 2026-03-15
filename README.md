# Non-blocking ds18b20 temp sensor example

This is an example/template of a bare metal, event triggered application that
runs tasks secuentally for STM32 boards using HAL library, in particular
STM32F103RB on NUCLEO-F103RB board.

In this case one of the tasks reads temperature from a ds18b20 sensor attached
to UART1 in Half-Duplex OneWire mode.

## Execution stack

The main loop calls the main app and then this app executes the attached tasks
secuentially in order: `task_sensor`, `task_system` and `task_actuator`.

## Debug

In Stm32CubeIDE Project Explorer view right click over the project, then debug
configurations. In Debugger tab set Debug probe as ST-LINK (OpenOCD) and after
pressing Show generator options setupReset Mode as Software system reset.
In Startup tab add 'monitor arm semihosting enable' to the initialization
commands. Finally Apply and Close (or Debug).
