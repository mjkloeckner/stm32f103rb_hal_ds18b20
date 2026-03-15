# app folder

In this folder the main app source code is stores. The main app consists of
three tasks that are run sequentially one after the other. Task sensor, system
and actuator. The app is updated on every tick (1 ms) and the app calls the
update method of every task.

## Files

### `app.*`

Endless loops, which execute tasks with fixed computing time. This 
sequential execution is only deviated from when an interrupt event occurs.
Cyclic Executive (Update by Time Code, period = 1mS)

### `task_sensor*`

Non-Blocking & Update By Time Code -> Sensor Modeling

### `task_system*`

Non-Blocking Code -> System Modeling

### `task_system_interface.*`

Non-Blocking Code

### `task_actuator*`

Non-Blocking & Update By Time Code -> Actuator Modeling

### `task_actuator_interface.*`

Non-Blocking Code

### `logger.*`

Utilities for Retarget "printf" to Console

### `dwt.h`

Utilities for Mesure "clock cycle" and "execution time" of code

### `systick.*`

Utilities for delay "microseconds"

## LICENSE

See the file LICENSE on this same folder for more details
