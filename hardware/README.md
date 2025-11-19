# Hardware

<div align="center">
<a href="./unit_sch_v_1_0_0_ue0112_ws28b20_matrix_rgb_8x16.pdf"><img src="resources/Schematics_icon.jpg?raw=false" width="200px"><br/>Schematic</a>
</div>

## Technical Specifications

### Electrical Characteristics

Unless noted, all parameter values are measured for a single LED.

<div align="center">

| **Parameter** |                 **Description**                  | **Min** | **Typ** | **Max** | **Unit** |
|:-------------:|:------------------------------------------------:|:-------:|:-------:|:-------:|:--------:|
|      Vdd      |       Input voltage to power on the module       |   3.3   |    -    |   5.5   |    V     |
|     Idd*      | Supply current (All LEDs at maximum power at 5V) |    -    |    -    |  2.56   |    A     |
|               |        All LEDs at maximum power at 3.3V         |    -    |    -    |  1.68   |    A     |
|      Vdi      |               Logic input Voltage                |  -0.4   |    -    | VDD+0.4 |    V     |
|      Vih      |           Input high threshold voltage           |   2.8   |    -    |    -    |    V     |
|      Vil      |           Input Low threshold voltage            |    -    |    -    |   1.6   |    V     |
|      Loh      |               DOUT output current                |    -    |   -35   |    -    |    mA    |
|      Lol      |              DOUT current injection              |    -    |   35    |    -    |    mA    |
|     Fpwm      |               Output PWM frequency               |    -    |    1    |    -    |   KHz    |
|      Iv       |               Light Intensity Red                |    -    |   200   |    -    |   mcd    |
|               |              Light Intensity Green               |    -    |   800   |    -    |   mcd    |
|               |               Light Intensity Blue               |    -    |   300   |    -    |   mcd    |
|      λD       |               Main Wavelength Red                |   620   |    -    |   630   |    nm    |
|               |              Main Wavelength Green               |   520   |    -    |   530   |    nm    |
|               |               Main Wavelength Blue               |   465   |    -    |   475   |    nm    |
|     2θ1/2     |                 Half light angle                 |    -    |   120   |    -    |   Deg    |

</div>

*Note: When powering the LEDs at 5V, their temperature may rise above 100°C at higher power levels. It is strongly recommended to implement optimal thermal management or to limit the use of LEDs at high brightness.
Also, be careful when powering via the JST connector; it is recommended to limit the operating current to less than 3A.

## Pinout

<div align="center">
    <a href="#"><img src="resources/unit_pinout_v_0_0_1_ue0094_icp10111_barometric_pressure_sensor_en.jpg" width="500px"><br/>Pinout</a>
    <br/>
    <br/>
    <br/>
</div>

## Pin & Connector Layout

<div align="center">

| Pin   | Voltage Level | Function                                                  |
|-------|---------------|-----------------------------------------------------------|
| VCC   | 3.3 V – 5.5 V | Provides power to the on-board regulator and sensor core. |
| GND   | 0 V           | Common reference for power and signals.                   |
| SDA   | 1.8 V to VCC  | Serial data line for I²C communications.                  |
| SCL   | 1.8 V to VCC  | Serial clock line for I²C communications.                 |

> **Note:** The module also includes a Qwiic/STEMMA QT connector carrying the same four signals (VCC, GND, SDA, SCL) for effortless daisy-chaining.

</div>

## Topology

<div align="center">
<a href="./resources/unit_topology_v_1_0_0_ue0112_ws28b20_matrix_rgb_8x16.png"><img src="./resources/unit_topology_v_1_0_0_ue0112_ws28b20_matrix_rgb_8x16.png" width="500px"><br/> Topology</a>
<br/>
<br/>
<br/>

| Ref.     | Description                              |
|----------|------------------------------------------|
| JP1      | 2.54 mm Pin Header                       |
| JP2      | 2.54 mm Pin Header                       |
| JP3      | 2.54 mm Pin Header                       |
| J1       | JST 1 mm Pitch Connector for Data Input  |
| J2       | JST 1 mm Pitch Connector for Data Output |
| L1-L64   | WS28B20 Neopixel for Matrix 1            |
| L65-L128 | WS28B20 Neopixel for Matrix 2            |
| SB1      | Solder Bridge to Join Both Matrices      |
</div>

## Dimensions

<div align="center">
<a href="./resources/unit_dimension_v_1_0_0_ue0112_ws28b20_matrix_rgb_8x16.png"><img src="./resources/unit_dimension_v_1_0_0_ue0112_ws28b20_matrix_rgb_8x16.png" width="500px"><br/> Dimensions</a>
</div>

## References

