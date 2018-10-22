<a id="library"></a>
# gbjBH1750FVI
Library for digital ambient light sensor BH1750FVI with two-wire (I2C) bus interface usually on board GY-302. Library enables changing address of the sensor dynamically, if ADDR pin of the sensor is connected to a microcontroller pin and changed programmatically accordingly as well as changing measurement mode.
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
- **gbjTwoWire**: I2C custom library loaded from the file *gbj_twowire.h*. The library [gbjBH1750FVI](#library) inherits common bus functionality from this library.


<a id="constants"></a>
## Constants
All constants are embedded into the class as static ones including result and error codes except constant defining hardware keypad equipment.

- **gbj\_bh1750fvi::VERSION**: Name and semantic version of the library.

Error codes as well as result code are inherited from the parent library [gbjTwoWire](#dependency).

<a id="addresses"></a>
#### Measurement modes
- **gbj\_bh1750fvi::ADDRESS\_LOW**: Sensor's address at floating or grounded ADDR pin (0x23).
- **gbj\_bh1750fvi::ADDRESS\_HIGH**: Sensor's address at floating or grounded ADDR pin (0x5C).

<a id="modes"></a>
#### Measurement modes
- **gbj\_bh1750fvi::CONTINUOUS\_HIGH**: Start measurement at 1 lx resolution. Measurement time is typically 120 ms.
- **gbj\_bh1750fvi::CONTINUOUS\_HIGH2**: Start measurement at 0.5 lx resolution. Measurement time is typically 120 ms.
- **gbj\_bh1750fvi::CONTINUOUS\_LOW**: Start measurement at 4 lx resolution. Measurement time is typically 16 ms.
- **gbj\_bh1750fvi::ONETIME\_HIGH**: Start measurement at 1 lx resolution. Measurement time is typically 120 ms. The sensor is automatically set to Power Down mode after measurement.
- **gbj\_bh1750fvi::ONETIME\_HIGH2**: Start measurement at 0.5 lx resolution. Measurement time is typically 120 ms. The sensor is automatically set to Power Down mode after measurement.
- **gbj\_bh1750fvi::ONETIME\_LOW**: Start measurement at 4 lx resolution. Measurement time is typically 16 ms. The sensor is automatically set to Power Down mode after measurement.


<a id="interface"></a>
## Interface

#### Main
- [gbj_bh1750fvi()](#gbj_bh1750fvi)
- [begin()](#begin)
- [powerOn()](#power)
- [powerDown()](#power)
- [reset()](#reset)
- [measureLight()](#measureLight)

#### Setters
- [setAddress()](#setAddress)
- [setMode()](#setMode)

#### Getters
- [getMode()](#getMode)
- [getLight()](#getLight)
- [getLightMSB()](#getLightByte)
- [getLightLSB()](#getLightByte)

Other possible setters and getters are inherited from the parent library [gbjTwoWire](#dependency) and described there.


<a id="gbj_bh1750fvi"></a>
## gbj_bh1750fvi()
#### Description
The library does not need special constructor and destructor, so that the inherited ones are good enough and there is no need to define them in the library, just use it with default or specific parameters as defined at constructor of parent library [gbjTwoWire](#dependency).
- Constructor sets parameters specific to the two-wire bus in general.
- All the constructor parameters can be changed dynamically with corresponding setters later in a sketch.

#### Syntax
    gbj_bh1750fvi(uint32_t clockSpeed, bool busStop, uint8_t pinSDA, uint8_t pinSCL);

#### Parameters
<a id="prm_busClock"></a>
- **clockSpeed**: Two-wire bus clock frequency in Hertz. If the clock is not from enumeration, it fallbacks to 100 kHz.
  - *Valid values*: gbj\_bh1750fvi::CLOCK\_100KHZ, gbj\_bh1750fvi::CLOCK\_400KHZ
  - *Default value*: gbj\_bh1750fvi::CLOCK\_100KHZ


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
  gbj_bh1750fvi Sensor = gbj_bh1750fvi(); // It is equivalent to
  gbj_bh1750fvi Sensor = gbj_bh1750fvi(gbj_bh1750fvi::CLOCK_100KHZ, true, D2, D1);
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
  - *Valid values*: [gbj\_bh1750fvi::ADDRESS\_LOW](#addresses), [gbj\_bh1750fvi::ADDRESS\_HIGH](#addresses), or pin state **HIGH** or **LOW**.
  - *Default value*: [gbj\_bh1750fvi::ADDRESS\_LOW](#addresses)
    - The default values is set to address corresponding to not wired (floating) ADDR pin, which is equivalent to the connection to the ground.
    - If input value is none of expected ones, the method fallbacks it to default address.
    - In fact, the ADDR pin is not aimed to utilize dynamic changing the device address, but to enable using two sensors on one two-wire bus.

<a id="prm_mode"></a>
- **mode**: Measurement mode from possible listed ones.
  - *Valid values*: [gbj\_bh1750fvi::MODE\_CONTINUOUS\_HIGH](#modes) ~ [gbj\_bh1750fvi::MODE\_ONETIME\_LOW](#modes)
  - *Default value*: [gbj\_bh1750fvi::MODE\_CONTINUOUS\_HIGH](#modes)
    - If input value is none of expected ones, the method fallbacks it to default measuring mode.

#### Returns
Some of [result or error codes](#constants).

#### Example
The method has all arguments defaulted and calling without any parameters is equivalent to the calling with all arguments set by corresponding constant with default value:

``` cpp
gbj_bh1750fvi Sensor = gbj_bh1750fvi();
setup()
{
    Sensor.begin();  // It is equivalent to
    Sensor.begin(gbj_bh1750fvi::ADDRESS_LOW, gbj_bh1750fvi::MODE_CONTINUOUS_HIGH);
}
```

If some argument after some defaulted arguments should have a specific value, use corresponding constants in place of those defaulted arguments, e.g.,

``` cpp
Sensor.begin(gbj_bh1750fvi::ADDRESS_HIGH, gbj_bh1750fvi::MODE_ONETIME_LOW);      // Specific measurement mode
```

Typical usage is just with default values without any arguments.
Specific values of arguments can be set by corresponding [setters](#interface).

#### See also
[setAddress()](#setAddress)

[setMode()](#setMode)

[Back to interface](#interface)


<a id="power"></a>
## powerOn(), powerDown()
#### Description
The particular method either activates (wakes up) or deactivates (sleeps down) a sensor.
- In active state a sensor waits for the measurement command.
- In sleeping state a sensor has minimal power consumption.

#### Syntax
    uint8_t powerOn();
    uint8_t powerDown();

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
The method resets the illuminance data register of the sensor and removes previous measurement result.

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
The method measures the ambient light intensity in lux at 16-bit resolution with accuracy determined by measurement mode while uses the parameters set by either the method [begin()](#begin) or setters. The method stores the measured value in the class instance object for repeating retrieval without need to measure again.
- Maximal light intensity value is 54612 lux, because of maximal unsigned integer value 65535 in the internal registers of the sensor and divided by measurement coefficient 1.2, i.e.,
  `65535 / 1.2 = 54612.5` and truncating decimal part.

#### Syntax
    uint16_t measureLight();

#### Parameters
None

#### Returns
Current ambient light intensity in the range 0 ~ 54612 lux or unreachable value 65535 (0xFFFF) signaling erroneous measurement. The corresponding [result or error code](#constants) is set as well.

#### See also
[getLight()](#getLight)

[getLightMSB(), getLightLSB()](#getLightByte)

[Back to interface](#interface)


<a id="setAddress"></a>
## setAddress()
#### Description
The method overrides the method of the parent class by transforming and sanitizing input value, which can be a state of the ADDR pin.
- If the input address is an address value, it should be just one of valid addresses [gbj\_bh1750fvi::ADDRESS\_LOW](#addresses), [gbj\_bh1750fvi::ADDRESS\_HIGH](#addresses).
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
Sensor.setAddress(digitalRead(PIN_BH1750FVI_ADDR));
Sensor.setAddress(gbj_bh1750fvi::ADDRESS_HIGH);
```

#### See also
[begin()](#begin)

[getAddress()](#getAddress)

[Back to interface](#interface)


<a id="setMode"></a>
## setMode()
#### Description
The method sets the measurement mode of the sensor. It should be one of defined [measuring modes](#modes).

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


<a id="getLight"></a>
## getLight()
#### Description
The method retrieves recently measured light intensity without need to measure again, e.g., for repeating usage without local variable.

#### Syntax
    uint16_t getLight();

#### Parameters
None

#### Returns
Recently measured light intensity in lux.

#### See also
[measureLight()](#measureLight)

[Back to interface](#interface)


<a id="getLightByte"></a>
## getLightMSB(), getLightLSB()
#### Description
The particular method retrieves the most or least significant byte of the raw sensor value from recent measurement. It is suitable for testing purposes or at very restrictive data transfers, e.g., over the air.

#### Syntax
    uint8_t getLightMSB();
    uint8_t getLightLSB();

#### Parameters
None

#### Returns
High or low byte of the recently measured sensor value word.

#### See also
[getLight()](#getLight)

[Back to interface](#interface)
