<a id="library"></a>
# gbjBH1750
Library for digital ambient light sensor BH1750 with two-wire (I2C) bus interface usually on board GY-302. Library enables changing address of the sensor dynamically, if ADDR pin of the sensor is connected to a microcontroller pin and changed programmatically accordingly as well as changing measurement mode.
- Sensor address is `0x23` for ADDR pin state with voltage <= 0.3 Vcc (digital LOW) or floating.
- Sensor address is `0x5C` for ADDR pin state with voltage >= 0.7 Vcc (digital HIGH).


#### Particle - Photon hardware configuration
- Connect microcontroller's pin `D0` to sensor's pin **SDA** (Serial Data).
- Connect microcontroller's pin `D1` to sensor's pin **SCL** (Serial Clock).
- Connect microcontroller's pin `D2` to sensor's pin **ADDR** (Addressing) in order to set its address programmatically.

#### AVR - Arduino UNO hardware configuration
- Connect microcontroller's pin `A4` to sensor's pin **SDA** (Serial Data).
- Connect microcontroller's pin `A5` to sensor's pin **SCL** (Serial Clock).
- Connect microcontroller's pin `D2` to sensor's pin **ADDR** (Addressing) in order to set its address programmatically.

#### Espressif - ESP8266, ESP32 default hardware configuration
- Connect microcontroller's pin `D2` to sensor's pin **SDA** (Serial Data).
- Connect microcontroller's pin `D1` to sensor's pin **SCL** (Serial Clock).
- Connect microcontroller's pin `D0` to sensor's pin **ADDR** (Addressing) in order to set its address programmatically.


<a id="credit"></a>
## Credit
Library has been inspired by the *Christopher Laws*'s library *BH1750-master version 1.1.3* but totally redesigned and rewritten.


<a id="dependency"></a>
## Dependency

#### Particle platform
- **Particle.h**: Includes alternative (C++) data type definitions.

#### Arduino platform
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.
- **TwoWire**: I2C system library loaded from the file *Wire.h*.

#### Custom Libraries
- **gbjTwoWire**: I2C custom library loaded from the file *gbj_twowire.h*. The library [gbjbh1750](#library) inherits common bus functionality from this library.


<a id="constants"></a>
## Constants
All constants are embedded into the class as static ones including result and error codes except constant defining hardware keypad equipment.

- **gbj\_bh1750::VERSION**: Name and semantic version of the library.

Error codes as well as result code are inherited from the parent library [gbjTwoWire](#dependency).

<a id="addresses"></a>
#### Measurement modes
- **gbj\_bh1750::ADDRESS\_GND**: Sensor's address at grounded ADDR pin (0x23).
- **gbj\_bh1750::ADDRESS\_FLOAT**: Sensor's address at floating ADDR pin (0x23). Always same as for grounded address pin, but present just for stress hardware configuration in a sketch.
- **gbj\_bh1750::ADDRESS\_VCC**: Sensor's address at ADDR pin connected to power voltage Vcc (0x5C).

<a id="modes"></a>
#### Measurement modes
- **gbj\_bh1750::CONTINUOUS\_HIGH**: Start measurement at 1 lx resolution. Measurement time is typically 120 ms.
- **gbj\_bh1750::CONTINUOUS\_HIGH2**: Start measurement at 0.5 lx resolution. Measurement time is typically 120 ms.
- **gbj\_bh1750::CONTINUOUS\_LOW**: Start measurement at 4 lx resolution. Measurement time is typically 16 ms.
- **gbj\_bh1750::ONETIME\_HIGH**: Start measurement at 1 lx resolution. Measurement time is typically 120 ms. The sensor is automatically set to Power Down mode after measurement.
- **gbj\_bh1750::ONETIME\_HIGH2**: Start measurement at 0.5 lx resolution. Measurement time is typically 120 ms. The sensor is automatically set to Power Down mode after measurement.
- **gbj\_bh1750::ONETIME\_LOW**: Start measurement at 4 lx resolution. Measurement time is typically 16 ms. The sensor is automatically set to Power Down mode after measurement.

The library increases calculated measurement time from datasheet values for particular measurement mode by safety margin 5% in order to provide a sensor sufficient time for conversion. Without it the measurement is not reliable.

The library does no specific error codes. All are inherited from the parent class.


<a id="interface"></a>
## Interface

#### Main
- [gbj_bh1750()](#gbj_bh1750)
- [begin()](#begin)
- [powerOn()](#power)
- [powerOff()](#power)
- [reset()](#reset)
- [measureLight()](#measureLight)
- [measureLightTyp()](#measureLightValue)
- [measureLightMin()](#measureLightValue)
- [measureLightMax()](#measureLightValue)

#### Setters
- [setAddress()](#setAddress)
- [setMode()](#setMode)
- [setTimingTyp()](#setTiming)
- [setTimingMax()](#setTiming)
- [setResolutionVal()](#setResolutionVal)
- [setResolutionTyp()](#setResolution)
- [setResolutionMin()](#setResolution)
- [setResolutionMax()](#setResolution)

#### Getters
- [getMode()](#getMode)
- [getSenseCoef()](#getSenseCoef)
- [getLightResult()](#getLightResult)
- [getLightTyp()](#getLightValue)
- [getLightMin()](#getLightValue)
- [getLightMax()](#getLightValue)
- [getTimingTyp()](#getTiming)
- [getTimingMax()](#getTiming)
- [getMeasurementTime()](#getMeasurementTime)
- [getMeasurementTimeTyp()](#getMeasurementTime)
- [getMeasurementTimeMax()](#getMeasurementTime)
- [getResolutionTyp()](#getResolution)
- [getResolutionMin()](#getResolution)
- [getResolutionMax()](#getResolution)
- [getSensitivityTyp()](#getSensitivity)
- [getSensitivityMin()](#getSensitivity)
- [getSensitivityMax()](#getSensitivity)
- [getAccuracyTyp()](#getAccuracy)
- [getAccuracyMin()](#getAccuracy)
- [getAccuracyMax()](#getAccuracy)

Other possible setters and getters are inherited from the parent library [gbjTwoWire](#dependency) and described there.


<a id="gbj_bh1750"></a>
## gbj_bh1750()
#### Description
The library does not need special constructor and destructor, so that the inherited ones are good enough and there is no need to define them in the library, just use it with default or specific parameters as defined at constructor of parent library [gbjTwoWire](#dependency).
- Constructor sets parameters specific to the two-wire bus in general.
- All the constructor parameters can be changed dynamically with corresponding setters later in a sketch.

#### Syntax
    gbj_bh1750(uint32_t clockSpeed, bool busStop, uint8_t pinSDA, uint8_t pinSCL);

#### Parameters
<a id="prm_busClock"></a>
- **clockSpeed**: Two-wire bus clock frequency in Hertz. If the clock is not from enumeration, it fallbacks to 100 kHz.
  - *Valid values*: gbj\_bh1750::CLOCK\_100KHZ, gbj\_bh1750::CLOCK\_400KHZ
  - *Default value*: gbj\_bh1750::CLOCK\_100KHZ


<a id="prm_busStop"></a>
- **busStop**: Logical flag about releasing bus after end of transmission.
  - *Valid values*: true, false
    - **true**: Releases the bus after data transmission and enables other master devices to control the bus.
    - **false**: Keeps connection to the bus and enables to begin further data transmission immediately.
  - *Default value*: true


<a id="prm_pinSDA"></a>
- **pinSDA**: Microcontroller's pin for serial data. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms for communication on the bus. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  - *Valid values*: positive integer
  - *Default value*: 4 (GPIO4, D2)


<a id="prm_pinSCL"></a>
- **pinSCL**: Microcontroller's pin for serial clock. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  - *Valid values*: positive integer
  - *Default value*: 5 (GPIO5, D1)

#### Returns
Object performing the sensor management.
The constructor cannot return [a result or error code](#constants) directly, however, it stores them in the instance object. The result can be tested in the operational code with the method [getLastResult()](#getLastResult), [isError()](#isError), or [isSuccess()](#isSuccess).

#### Example
The method has all arguments defaulted and calling without any parameters is equivalent to the calling with all arguments set by corresponding constant with default value:

```cpp
  gbj_bh1750 Sensor = gbj_bh1750(); // It is equivalent to
  gbj_bh1750 Sensor = gbj_bh1750(gbj_bh1750::CLOCK_100KHZ, true, D2, D1);
```

[Back to interface](#interface)


<a id="begin"></a>
## begin()
#### Description
The method takes, sanitizes, and stores sensor parameters to a class instance object and initiates two-wire bus.
- The method sets parameters specific to the sensor itself.
- All the method parameters can be changed dynamically with corresponding [setters](#interface) later in a sketch.
- The method enables to set the sensor's address according to the ADDR pin, if it is wired to a microcontroller's pin.

#### Syntax
    uint8_t begin(uint8_t address, uint8_t mode);

#### Parameters
<a id="prm_address"></a>
- **address**: One of two possible 7 bit addresses of the sensor or state of the ADDR pin.
  - *Valid values*: [gbj\_bh1750::ADDRESS\_LOW](#addresses), [gbj\_bh1750::ADDRESS\_FLOAT](#addresses), [gbj\_bh1750::ADDRESS\_HIGH](#addresses), or pin state **HIGH** or **LOW**.
  - *Default value*: [gbj\_bh1750::ADDRESS\_LOW](#addresses)
    - The default values is set to address corresponding to not wired (floating) ADDR pin, which is equivalent to the connection to the ground.
    - If input value is none of expected ones, the method fallbacks it to default address.
    - In fact, the ADDR pin is not aimed to utilize dynamic changing the device address, but to enable using two sensors on one two-wire bus.

<a id="prm_mode"></a>
- **mode**: Measurement mode from possible listed ones.
  - *Valid values*: [gbj\_bh1750::MODE\_CONTINUOUS\_HIGH](#modes) ~ [gbj\_bh1750::MODE\_ONETIME\_LOW](#modes)
  - *Default value*: [gbj\_bh1750::MODE\_CONTINUOUS\_HIGH](#modes)
    - If input value is none of expected ones, the method fallbacks it to default measuring mode.

#### Returns
Some of [result or error codes](#constants).

#### Example
The method has all arguments defaulted and calling without any parameters is equivalent to the calling with all arguments set by corresponding constant with default value:

``` cpp
gbj_bh1750 Sensor = gbj_bh1750();
setup()
{
    Sensor.begin();  // It is equivalent to
    Sensor.begin(gbj_bh1750::ADDRESS_GND, gbj_bh1750::MODE_CONTINUOUS_HIGH);
}
```

If some argument after some defaulted arguments should have a specific value, use corresponding constants in place of those defaulted arguments, e.g.,

``` cpp
Sensor.begin(gbj_bh1750::ADDRESS_VCC, gbj_bh1750::MODE_ONETIME_LOW);      // Specific measurement mode
```

Typical usage is just with default values without any arguments.
Specific values of arguments can be set by corresponding [setters](#interface).

#### See also
[setAddress()](#setAddress)

[setMode()](#setMode)

[Back to interface](#interface)


<a id="power"></a>
## powerOn(), powerOff()
#### Description
The particular method either activates (wakes up) or deactivates (sleeps down) a sensor.
- In active state a sensor waits for the measurement command.
- In sleeping state a sensor has minimal power consumption.

#### Syntax
    uint8_t powerOn();
    uint8_t powerOff();

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[reset()](#reset)

[Back to interface](#interface)


<a id="reset"></a>
## reset()
#### Description
The method resets the illuminance data register of the sensor, which removes previous measurement result, and sets previous measurement mode.

#### Syntax
    uint8_t reset();

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[measureLight()](#measureLight)

[Back to interface](#interface)


<a id="measureLight"></a>
## measureLight()
#### Description
The method measures the ambient light intensity and calculates it in lux at all measurement accuracies (typical, minimal, maximal) with resolution set before exactly or by measurement mode.
- The method stores the measured and calculated values in the class instance object for repeating retrieval without need to measure again. Particular values can be obtain with corresponding getters.
- The method is useful in a sketch when light intensity at all accuracies are utilized for its confidence interval.

#### Syntax
    uint8_t measureLight();

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[measureLightTyp(), measureLightMin(), measureLightMax()](#measureLightValue)

[getLightTyp(), getLightMin(), getLightMax()](#getLightValue)

[getLightResult()](#getLightResult)

[Back to interface](#interface)


<a id="measureLightValue"></a>
## measureLightTyp(), measureLightMin(), measureLightMax()
#### Description
The particular method measures the ambient light and calculates intensity in lux for corresponding accuracy (typical, minimal, maximal). It subsequently uses the method [measureLight()](#measureLightValue) and corresponding method [getLightXXX()](#getLightValue).
- Particular methods is useful in a sketch when the light intensity at just one accuracy is needed.

#### Syntax
    float measureLightTyp();
    float measureLightMin();
    float measureLightMax();

#### Parameters
None

#### Returns
Current ambient light intensity in lux at corresponding accuracy. The [result or error code](#constants) is set as well.

#### See also
[getLight()](#getLight)

[getLightMSB(), getLightLSB()](#getLightByte)

[Back to interface](#interface)


<a id="setAddress"></a>
## setAddress()
#### Description
The method overrides the method of the parent class by transforming and sanitizing input value, which can be a state of the ADDR pin.
- If the input address is an address value, it should be just one of valid addresses [gbj\_bh1750::ADDRESS\_LOW](#addresses), [gbj\_bh1750::ADDRESS\_FLOAT](#addresses), [gbj\_bh1750::ADDRESS\_HIGH](#addresses).
- If the input address is the ADDR pin state, it can be either HIGH or LOW.
- In fact, the ADDR pin is not aimed to utilize dynamic changing the device address, but to enable using two sensors on one two-wire bus.

#### Syntax
    uint8_t setAddress(uint8_t address);

#### Parameters
- **address**: The address value or the ADDR pin state, so the same range as the argument [address](#prm_address) in the method [begin()](#begin).

#### Returns
Some of [result or error codes](#constants).

#### Example
```cpp
Sensor.setAddress(digitalRead(PIN_BH1750_ADDR));
Sensor.setAddress(gbj_bh1750::ADDRESS_VCC);
```

#### See also
[begin()](#begin)

[Back to interface](#interface)


<a id="setMode"></a>
## setMode()
#### Description
The method sets the measurement mode of the sensor. It should be one of defined [measuring modes](#modes).
- If a low measuring mode is selected, the method sets the measurement time for typical value of measurement time register.
- For selected mode and current resolution the method calculates measurement time and measurement sensitivity.

#### Syntax
    uint8_t setMode(uint8_t mode);

#### Parameters
- **mode**: The same as the argument [mode](#prm_mode) in the method [begin()](#begin).

#### Returns
Some of [result or error codes](#constants).

#### See also
[begin()](#begin)

[getMode()](#getMode)

[Back to interface](#interface)


<a id="getMode"></a>
## getMode()
#### Description
The method returns the current measurement mode of the sensor stored in the class instance object.

#### Syntax
    uint8_t getMode();

#### Parameters
None

#### Returns
Current measurement mode of the sensor.

#### See also
[setMode()](#setMode)

[Back to interface](#interface)


<a id="getSenseCoef"></a>
## getSenseCoef()
#### Description
The method returns the current sensitivity coefficient defined by ratio of
current set value of the measurement time register and its typical value.

#### Syntax
    float getSenseCoef();

#### Parameters
None

#### Returns
Current sensitivity coefficient.

#### See also
[setResolutionVal()](#setResolutionVal)

[Back to interface](#interface)


<a id="getLightValue"></a>
## getLightTyp(), getLightMin(), getLightMax()
#### Description
The particular method retrieves recently measured and calculated light intensity for corresponding accuracy without need to measure again, e.g., for repeating usage without local variable in a sketch.

#### Syntax
    float getLightTyp();
    float getLightMin();
    float getLightMax();

#### Parameters
None

#### Returns
Recently measured light intensity in lux.

#### See also
[measureLight()](#measureLight)

[Back to interface](#interface)


<a id="getLightResult"></a>
## getLightResult()
#### Description
The method provides binary content of the sensor's data register as a raw light intensity value from recent measurement. It is suitable for testing purposes or calibration.

#### Syntax
    uint16_t getLightResult();

#### Parameters
None

#### Returns
Binary word of the recently measured sensor value.

#### See also
[getLightTyp(), getLightMin(), getLightMax()](#getLightValue)

[Back to interface](#interface)


<a id="setTiming"></a>
## setTimingTyp(), setTimingMax()
#### Description
The particular method sets a flag of the internal library instance determining, whether either typical or maximal measurement time values should be used at measurement by the sensor.

#### Syntax
    void setTimingTyp();
    void setTimingMax();

#### Parameters
None

#### Returns
None

#### See also
[getTimingTyp(), getTimingMax()](#getTiming)

[Back to interface](#interface)


<a id="getTiming"></a>
## getTimingTyp(), getTimingMax()
#### Description
The particular method returns a flag about currently used typical or maximal measurement time values for measurement by the sensor.
- Although both methods are complement, they are present for enabling better readability in sketches.

#### Syntax
    bool getTimingTyp();
    bool getTimingMax();

#### Parameters
None

#### Returns
Boolean flag about used typical or maximal measurement time values.

#### See also
[setTimingTyp(), setTimingMax()](#setTiming)

[Back to interface](#interface)


<a id="getMeasurementTime"></a>
## getMeasurementTime(), getMeasurementTimeTyp(), getMeasurementTimeMax()
#### Description
The particular method returns corresponding measurement time in milliseconds for current measurement mode and resolution.
- The methods `getMeasurementTimeTyp()` and `getMeasurementTimeMax()` are calculated for particular sensor's accuracy and current value of sensor's measurement time register.
- The method `getMeasurementTime()` provides measurement time used for real light measurement. Its minimal value is limited to the typical measurement time for current measurement mode even if calculated time for lower resolutions is usually shorter. It ensures sufficient time for the sensor for processing measurement.

#### Syntax
    uint16_t getMeasurementTime();
    uint16_t getMeasurementTimeTyp();
    uint16_t getMeasurementTimeMax();

#### Parameters
None

#### Returns
Measurement time in milliseconds for current setting of the sensor.

#### See also
[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)


<a id="setResolutionVal"></a>
## setResolutionVal()
#### Description
The method writes provided value to the measurement time register of the sensor.
- The measurement resolution in conjunction with sensor's current measurement accuracy (typical, maximal) and current measurement mode determines measurement time and measurement sensitivity in `lux per bit count`.
- The method limits minimal measurement time to the typical one for current measurement mode.
- The method forces typical value of the measurement time register at low measurement modes.

#### Syntax
    uint8_t setResolutionVal(uint8_t mtreg);

#### Parameters
- **mtreg**: Desired value of measurement time register.
  - *Valid values*: 31 ~ 254, typical 69
  - *Default value*: none
    - If current measurement mode is for low resolution, the value is always typical one.

#### Returns
Some of [result or error codes](#constants).

#### See also
[setResolutionTyp(), setResolutionMin(), setResolutionMax()](#setResolution)

[Back to interface](#interface)


<a id="setResolution"></a>
## setResolutionTyp(), setResolutionMin(), setResolutionMax()
#### Description
The particular method sets measurement resolution by writing corresponding value to the measurement time register of the sensor.
- Those limits have nothing common with typical, minimal, or maximal values of parameters from a datasheet. They are just minimal, typical (default), and maximal values of the measurement time register.
- The measurement resolution is expressed in `bit count per lux`. It determines what binary value of the sensor's data register corresponds to 1 lx of measured ambient illuminance.
- The measurement resolution in conjunction with sensor's measurement accuracy determines measurement sensitivity in `lux per bit count`.
- The typical resolution is determined by the measurement time register value `69`.
- The minimal resolution is determined by the measurement time register value `31` and corresponds to 0.45 (31/69) fraction of typical resolution. This resolution is useful for measuring the highest light intensity up to 100000 lx, theoretically to 151946 lx at minimal datasheet resolution 0.96 bit count per lux and maximal illuminance register value 65535, i.e., `69 / 31 / 0.96 * 65535`.
- The maximal resolution is determined by the measurement time register value `254` and corresponds to 3.68 (254/69) multiple of typical resolution. This resolution is useful for measuring the lowest light intensity (darkness) down to 0.11 lx in conjunction with double high measurement mode, theoretically from 0.09 lx at maximal datasheet resolution 1.44 bit count per lux, double sensitivity, and minimal illuminance register value 1, i.e., `69 / 254 / 2 / 1.44 * 1`.
- Methods utilize the method [setResolutionVal()](#setResolutionVal) for writing particular value to the sensor's measurement time register.

#### Syntax
    uint8_t setResolutionTyp();
    uint8_t setResolutionMin();
    uint8_t setResolutionMax();

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[setResolutionVal()](#setResolutionVal)

[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)


<a id="getResolution"></a>
## getResolutionTyp(), getResolutionMin(), getResolutionMax()
#### Description
The particular method returns currently set measurement resolution in `bit count per lux`.

#### Syntax
    float getResolutionTyp();
    float getResolutionMin();
    float getResolutionMax();

#### Parameters
None

#### Returns
Current measurement resolution in `bit count per lux`.

#### See also
[setResolutionTyp(), setResolutionMin(), setResolutionMax()](#setResolution)

[Back to interface](#interface)


<a id="getSensitivity"></a>
## getSensitivityTyp(), getSensitivityMin(), getSensitivityMax()
#### Description
The particular method returns current measurement sensitivity.
- The measurement sensitivity is expressed in `lux per bit count`. It determines what light intensity corresponds to 1 bit of sensor's data register.
- The measurement sensitivity is the reciprocal value to the measurement resolution.

#### Syntax
    float getSensitivityTyp();
    float getSensitivityMin();
    float getSensitivityMax();

#### Parameters
None

#### Returns
Current measurement sensitivity in `lux per bit count`.

#### See also
[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)


<a id="getAccuracy"></a>
## getAccuracyTyp(), getAccuracyMin(), getAccuracyMax()
#### Description
The particular method returns corresponding sensor's accuracy (typical, minimal, maximal) as the data sheet them declares in `bit count per lux`. It is a factory measurement resolution at typical value of measurement time register.

#### Syntax
    float getAccuracyTyp();
    float getAccuracyMin();
    float getAccuracyMax();

#### Parameters
None

#### Returns
Factory sensor accuracy in `bit count per lux`.

#### See also
[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)
