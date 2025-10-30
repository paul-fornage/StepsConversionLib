# StepsConversionLib

Example usage:

```c++
#define STEPS_PER_MOTOR_REV_MACRO 800
#define MOTOR_REVS_PER_PINION_REV_MACRO 100
#define PINION_DIAMETER_MM_MACRO 67.91
#include "StepsConversions.h"
#undef STEPS_PER_MOTOR_REV_MACRO
#undef MOTOR_REVS_PER_PINION_REV_MACRO
#undef PINION_DIAMETER_MM_MACRO
```

### Common values (for me)

- pinions
  - ATLANTA 24 99 232 (helical): 67.91 mm
  - ANDANTEX A30-021-300: 48 mm
- Gearbox
  - 100:1
- Motor
  - 800 steps per revolution

