# gbjBH1750

Library for digital ambient light sensor `BH1750` with `two-wire` (also known as <abbr title='Inter-Integrated Circuit'>I2C</abbr>) bus interface usually on board `GY-302`.
* Sensor address is `0x23` for ADDR pin state with voltage <= 0.3 Vcc (digital LOW) or floating.
* Sensor address is `0x5C` for ADDR pin state with voltage >= 0.7 Vcc (digital HIGH).


#### Particle - Photon hardware configuration
* Connect microcontroller's pin `D0` to sensor's pin **SDA** (Serial Data).
* Connect microcontroller's pin `D1` to sensor's pin **SCL** (Serial Clock).

#### AVR - Arduino UNO hardware configuration
* Connect microcontroller's pin `A4` to sensor's pin **SDA** (Serial Data).
* Connect microcontroller's pin `A5` to sensor's pin **SCL** (Serial Clock).

#### Espressif - ESP8266, ESP32 default hardware configuration
* Connect microcontroller's pin `D2` to sensor's pin **SDA** (Serial Data).
* Connect microcontroller's pin `D1` to sensor's pin **SCL** (Serial Clock).

>Connect sensor's pin **ADDR** (Addressing) either to VCC (power supply) rail, or GND (ground) rail, or leave it unconnected in order to set its address. It is neccessary for using two sensors on the same I2C bus.


<a id="dependency"></a>

## Dependency

#### Particle platform
* **Particle.h**: Includes alternative (C++) data type definitions.

#### Arduino platform
* **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
* **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.
* **TwoWire**: I2C system library loaded from the file `Wire.h`.

#### Custom Libraries
* **gbjTwoWire**: I2C custom library loaded from the file `gbj_twowire.h`, which provides common bus functionality.


<a id="constants"></a>

## Constants
The library does not have specific error codes. Error codes as well as result code are inherited from the parent library only. The result code and error codes can be tested in the operational code with its method `getLastResult()`, `isError()` or `isSuccess()`.


<a id="addresses"></a>

#### Sensor addresses
* **Addresses::ADDRESS\_GND**: Sensor's address at grounded ADDR pin.
* **Addresses::ADDRESS\_VCC**: Sensor's address at ADDR pin connected to power supply voltage Vcc.
* **Addresses::ADDRESS\_FLOAT**: Sensor's address at floating ADDR pin. Always same as for grounded address pin, but present just for hardware configuration in a sketch.


<a id="modes"></a>

#### Measurement modes
* **Modes::CONTINUOUS\_HIGH**: Start measurement at `1 lx` resolution. Measurement time is typically `120 ms`.
* **Modes::CONTINUOUS\_HIGH2**: Start measurement at `0.5 lx` resolution. Measurement time is typically `120 ms`.
* **Modes::CONTINUOUS\_LOW**: Start measurement at `4 lx` resolution. Measurement time is typically `16 ms`.
* **Modes::ONETIME\_HIGH**: Start measurement at `1 lx` resolution. Measurement time is typically `120 ms`. The sensor is automatically set to _Power Down_ mode after measurement.
* **Modes::ONETIME\_HIGH2**: Start measurement at `0.5 lx` resolution. Measurement time is typically `120 ms`. The sensor is automatically set to _Power Down_ mode after measurement.
* **Modes::ONETIME\_LOW**: Start measurement at `4 lx` resolution. Measurement time is typically `16 ms`. The sensor is automatically set to _Power Down_ mode after measurement.

The library increases calculated measurement time from datasheet values for particular measurement mode by safety margin 5% in order to provide a sensor sufficient time for conversion. Without it the measurement is not reliable.

### Referencing constants
In a sketch the constants can be referenced in following forms:
* **Static constant** in the form `gbj_bh1750::<enumeration>::<constant>` or shortly `gbj_bh1750::<constant>`, e.g., _gbj_bh1750::Addresses::ADDRESS\_GND_ or _gbj_bh1750::ADDRESS\_GND_.
* **Instance constant** in the form `<object>.<constant>`, e.g., _sensor.ADDRESS\_GND_.
```cpp
gbj_bh1750 sensor = gbj_bh1750(sensor.CLOCK_400KHZ);
String getModeName()
{
  String measurementType;
  switch (sensor.getMode())
  {
    case sensor.MODE_CONTINUOUS_HIGH:
      measurementType = "Continuous High";
      break;
    ...
  }
  return measurementType;
}
```


<a id="interface"></a>

## Interface

#### Main
* [gbj_bh1750()](#gbj_bh1750)
* [begin()](#begin)
* [powerOn()](#power)
* [powerOff()](#power)
* [reset()](#reset)
* [measureLight()](#measureLight)
* [measureLightTyp()](#measureLightValue)
* [measureLightMin()](#measureLightValue)
* [measureLightMax()](#measureLightValue)

#### Setters
* [setAddress()](#setAddress)
* [setMode()](#setMode)
* [setTimingTyp()](#setTiming)
* [setTimingMax()](#setTiming)
* [setResolutionTyp()](#setResolution)
* [setResolutionMin()](#setResolution)
* [setResolutionMax()](#setResolution)

#### Getters
* [getMode()](#getMode)
* [getSenseCoef()](#getSenseCoef)
* [getLightResult()](#getLightResult)
* [getLightTyp()](#getLightValue)
* [getLightMin()](#getLightValue)
* [getLightMax()](#getLightValue)
* [getTimingTyp()](#getTiming)
* [getTimingMax()](#getTiming)
* [getMeasurementTime()](#getMeasurementTime)
* [getMeasurementTimeTyp()](#getMeasurementTime)
* [getMeasurementTimeMax()](#getMeasurementTime)
* [getResolutionTyp()](#getResolution)
* [getResolutionMin()](#getResolution)
* [getResolutionMax()](#getResolution)
* [getSensitivityTyp()](#getSensitivity)
* [getSensitivityMin()](#getSensitivity)
* [getSensitivityMax()](#getSensitivity)
* [getAccuracyTyp()](#getAccuracy)
* [getAccuracyMin()](#getAccuracy)
* [getAccuracyMax()](#getAccuracy)

Other possible setters and getters are inherited from the parent library [gbjTwoWire](#dependency) and described there.


<a id="gbj_bh1750"></a>

## gbj_bh1750()

#### Description
The library does not need special constructor and destructor, so that the inherited ones are good enough and there is no need to define them in the library, just use it with default or specific parameters as defined at constructor of parent library [gbjTwoWire](#dependency).
* Constructor sets parameters specific to the two-wire bus in general.
* All the constructor parameters can be changed dynamically with corresponding setters later in a sketch.

#### Syntax
    gbj_bh1750(ClockSpeeds clockSpeed, bool busStop, uint8_t pinSDA, uint8_t pinSCL)

#### Parameters
* **clockSpeed**: Two-wire bus clock frequency in Hertz.
  * *Valid values*:ClockSpeeds::CLOCK\_100KHZ, ClockSpeeds::CLOCK\_400KHZ
  * *Default value*: ClockSpeeds::CLOCK\_100KHZ

* **busStop**: Logical flag about releasing bus after end of transmission.
  * *Valid values*: true, false
    * **true**: Releases the bus after data transmission and enables other master devices to control the bus.
    * **false**: Keeps connection to the bus and enables to begin further data transmission immediately.
  * *Default value*: true

* **pinSDA**: Microcontroller's pin for serial data. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms for communication on the bus. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  * *Valid values*: positive integer
  * *Default value*: 4 (GPIO4, D2)

* **pinSCL**: Microcontroller's pin for serial clock. It is not a board pin but GPIO number. For hardware two-wire bus platforms it is irrelevant and none of methods utilizes this parameter for such as platforms. On the other hand, for those platforms the parameters might be utilized for storing some specific attribute in the class instance object.
  * *Valid values*: positive integer
  * *Default value*: 5 (GPIO5, D1)

#### Returns
Object performing the sensor management.
The constructor cannot return [a result or error code](#constants) directly, however, it stores them in the instance object.

#### Example
The method has all arguments defaulted and calling without any parameters is equivalent to the calling with all arguments set by corresponding constant with default value:

```cpp
  gbj_bh1750 sensor = gbj_bh1750(); // It is equivalent to
  gbj_bh1750 sensor = gbj_bh1750(gbj_bh1750::CLOCK_100KHZ, true, D2, D1);
```

[Back to interface](#interface)


<a id="begin"></a>

## begin()

#### Description
The method takes, sanitizes, and stores sensor parameters to a class instance object and initiates two-wire bus.
* The method sets parameters specific to the sensor itself.
* All the method parameters can be changed dynamically with corresponding [setters](#interface) later in a sketch.
* The method enables to set the sensor's address according to the ADDR pin, if it is wired to a microcontroller's pin.

#### Syntax
    ResultCodes begin(Addresses address, Modes mode)

#### Parameters
* **address**: One of two possible 7 bit addresses of the sensor corresponding to the ADDR pin connection.
  * *Valid values*: [Addresses::ADDRESS\_GND, Addresses::ADDRESS\_VCC, Addresses::ADDRESS\_FLOAT](#addresses)
  * *Default value*: [_FLOAT::ADDRESS\_GND](#addresses)

* **mode**: Measurement mode from possible listed ones.
  * *Valid values*: [Modes::MODE\_CONTINUOUS\_HIGH](#modes) ~ [Modes::MODE\_ONETIME\_LOW](#modes)
  * *Default value*: [Modes::MODE\_CONTINUOUS\_HIGH](#modes)

#### Returns
Some of [result or error codes](#constants).

#### Example
The method has all arguments defaulted and calling without any parameters is equivalent to the calling with all arguments set by corresponding constant with default value:

``` cpp
gbj_bh1750 sensor = gbj_bh1750();
setup()
{
    sensor.begin();  // It is equivalent to
    sensor.begin(sensor.ADDRESS_GND, sensor.MODE_CONTINUOUS_HIGH);
}
```

If some argument after some defaulted arguments should have a specific value, use corresponding constants in place of those defaulted arguments, e.g.,

``` cpp
sensor.begin(sensor.ADDRESS_VCC, sensor.MODE_ONETIME_LOW);      // Specific measurement mode
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
* In active state a sensor waits for the measurement command.
* In sleeping state a sensor has minimal power consumption.

#### Syntax
    ResultCodes powerOn()
    ResultCodes powerOff()

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
    ResultCodes reset()

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
* The method stores the measured and calculated values in the class instance object for repeating retrieval without need to measure again. Particular values can be obtain with corresponding getters.
* The method is useful in a sketch when light intensity at all accuracies are utilized for its confidence interval.

#### Syntax
    ResultCodes measureLight()

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
* Particular methods is useful in a sketch when the light intensity at just one accuracy is needed.

#### Syntax
    float measureLightTyp()
    float measureLightMin()
    float measureLightMax()

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
The method sets the new address of the sensor, e.g., at dynamical changing ADDR pin state by a microcontroller's pin.

#### Syntax
    ResultCodes setAddress(Addresses address)

#### Parameters
* **address**: The address value of the same range as the same argument in the method [begin()](#begin).

#### Returns
Some of [result or error codes](#constants).

#### Example
```cpp
sensor.setAddress(digitalRead(PIN_BH1750_ADDR) ? sensor.ADDRESS_VCC : sensor.ADDRESS_GND);
sensor.setAddress(sensor.ADDRESS_VCC);
```

#### See also
[begin()](#begin)

[Back to interface](#interface)


<a id="setMode"></a>

## setMode()

#### Description
The method sets the measurement mode of the sensor. It should be one of defined [measuring modes](#modes).
* If a low measuring mode is selected, the method sets the measurement time for typical value of measurement time register.
* For selected mode and current resolution the method calculates measurement time and measurement sensitivity.

#### Syntax
    ResultCodes setMode(Modes mode)

#### Parameters
* **mode**: The mode code of the same range as the same argument in the method [begin()](#begin).

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
    Modes getMode()

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
    float getSenseCoef()

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
    float getLightTyp()
    float getLightMin()
    float getLightMax()

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
    uint16_t getLightResult()

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
    void setTimingTyp()
    void setTimingMax()

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
* Although both methods are complement, they are present for enabling better readability in sketches.

#### Syntax
    bool getTimingTyp()
    bool getTimingMax()

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
* The methods `getMeasurementTimeTyp()` and `getMeasurementTimeMax()` are calculated for particular sensor's accuracy and current value of sensor's measurement time register.
* The method `getMeasurementTime()` provides measurement time used for real light measurement. Its minimal value is limited to the typical measurement time for current measurement mode even if calculated time for lower resolutions is usually shorter. It ensures sufficient time for the sensor for processing measurement.

#### Syntax
    uint16_t getMeasurementTime()
    uint16_t getMeasurementTimeTyp()
    uint16_t getMeasurementTimeMax()

#### Parameters
None

#### Returns
Measurement time in milliseconds for current setting of the sensor.

#### See also
[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)


<a id="setResolution"></a>

## setResolutionTyp(), setResolutionMin(), setResolutionMax()

#### Description
The particular method sets measurement resolution by writing corresponding value to the measurement time register of the sensor.
* Those limits have nothing common with typical, minimal, or maximal values of parameters from a datasheet. They are just minimal, typical (default), and maximal values of the measurement time register.
* The measurement resolution is expressed in `bit count per lux`. It determines what binary value of the sensor's data register corresponds to 1 lx of measured ambient illuminance.
* The measurement resolution in conjunction with sensor's measurement accuracy determines measurement sensitivity in `lux per bit count`.
* The typical resolution is determined by the measurement time register value `69`.
* The minimal resolution is determined by the measurement time register value `31` and corresponds to 0.45 (31/69) fraction of typical resolution. This resolution is useful for measuring the highest light intensity up to 100000 lx, theoretically to 151946 lx at minimal datasheet resolution 0.96 bit count per lux and maximal illuminance register value 65535, i.e., `69 / 31 / 0.96 * 65535`.
* The maximal resolution is determined by the measurement time register value `254` and corresponds to 3.68 (254/69) multiple of typical resolution. This resolution is useful for measuring the lowest light intensity (darkness) down to 0.11 lx in conjunction with double high measurement mode, theoretically from 0.09 lx at maximal datasheet resolution 1.44 bit count per lux, double sensitivity, and minimal illuminance register value 1, i.e., `69 / 254 / 2 / 1.44 * 1`.

#### Syntax
    ResultCodes setResolutionTyp()
    ResultCodes setResolutionMin()
    ResultCodes setResolutionMax()

#### Parameters
None

#### Returns
Some of [result or error codes](#constants).

#### See also
[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)


<a id="getResolution"></a>

## getResolutionTyp(), getResolutionMin(), getResolutionMax()

#### Description
The particular method returns currently set measurement resolution in `bit count per lux`.

#### Syntax
    float getResolutionTyp()
    float getResolutionMin()
    float getResolutionMax()

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
* The measurement sensitivity is expressed in `lux per bit count`. It determines what light intensity corresponds to 1 bit of sensor's data register.
* The measurement sensitivity is the reciprocal value to the measurement resolution.

#### Syntax
    float getSensitivityTyp()
    float getSensitivityMin()
    float getSensitivityMax()

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
    float getAccuracyTyp()
    float getAccuracyMin()
    float getAccuracyMax()

#### Parameters
None

#### Returns
Factory sensor accuracy in `bit count per lux`.

#### See also
[getResolutionTyp(), getResolutionMin(), getResolutionMax()](#getResolution)

[Back to interface](#interface)
