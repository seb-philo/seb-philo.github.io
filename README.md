# Design of an Intelligent Jumping Robot

MEng Mechanical Engineering with Mechatronics Final Project. The aim of which was to create a multi-locomotion robot with versatile application. I lead this project as chairman, achieving 75%.

<img width="470" height="405" alt="image" src="https://github.com/user-attachments/assets/bcdc8670-aa92-4c57-9a78-10de24b98711" />


## Summary
The project integrates a limbed carbon fibre jumping mechanism, which generates high releases forces once compressed - an adaption of mechanism with proven success throughout research, but requiring heavy modification to suit our requirements - at low additional mass to the resulting wheeled robot. This is paired with a unique electromagnetic latching system, using high tensile string - wound on a actuated pulley mechanism - to compress the carbon limbs, allowing for the electromagnet to activate, and be primed for release at will. These mechanisms are mounted on to an ultra-light carbon-PLA chassis, with optimised TPU airless tyres - giving extreme impact resistance, and standard electronics, allowing 2.11m/s differential-drive speed and remote control from 1km distance.
The code for the wireless control, for driving, latch winding, jump release, etc:

[`robot-reciever.cpp`](https://github.com/seb-philo/jumping-robot/blob/main/robot-reciever.cpp)
[`controller-transmitter`](https://github.com/seb-philo/jumping-robot/blob/main/controller-transmitter.cpp)

<img width="2066" height="1779" alt="Picture2" src="https://github.com/user-attachments/assets/70fb4da6-2977-4b1d-a411-a80cd2085369" />


## Jumping Mechanism Design
As well as leading the team as a whole, I designed the jumping mechanism.
Inspiration for this jumping mechanism came from a range of robots which adapt this kind of latch and release system; the best performing jumpera use a set of carbon fibre beams, buckling them to store strain energy, ready to release. 
High flexural modulus, and strength carbon was selected, and combined with TPU components to constrain in the desired planes.
With additional components, and the additional mechanisms of the robot considered, the mechanism was optimised through: CAD, FEA, 3D printed prototyping, and stress testing.


<img width="602" height="189" alt="image" src="https://github.com/user-attachments/assets/b85c2bdc-505a-4f44-b396-7162cbb7e3a1" />


<img width="941" height="529" alt="image" src="https://github.com/user-attachments/assets/580a2369-7557-4d5f-a861-e1a60f357fb6" />


<img width="602" height="230" alt="image" src="https://github.com/user-attachments/assets/119db056-a513-4235-a5ed-3ed8ed52e85f" />


Once optimised, this jumping mechanism alone, was validated through real testing. Using high-speed cameras, the vertical jump performance data was collected. Initially, the 4-limb mechanism reached a height of 1.77m , following this, the mechanism was able to be optimised further, adding an additional 8-limbs, improving to 3.63m (12.5x height). Importantly, this mechanism was able to jump with payload capacity, crucially over 1m at 1000g payload, a ket specification point.


<img width="937" height="540" alt="image" src="https://github.com/user-attachments/assets/8910c2da-35c7-41dc-b433-2b7fb4b04935" />


## Improvements
Although an 8-month project, we left knowing this project could go on far longer. Some improvements I suggested include:

Predictable height control, through the use of a PID controller and variable height latch, recieving input of desired height and determining the level of compression required.


<img width="470" height="437" alt="image" src="https://github.com/user-attachments/assets/98d9043e-9ea4-49eb-bfcc-dea5bc33a93e" />


Variable Centre of Mass; a key aim of this robot was to be able to jump, and land, whilst in motion. Although this was possible, to perfect this the robot would need to land the correct way up to continue driving, I suggested a variable centre of mass to control rotation throughout the jump time, this could be controlled through the use of an IMU sensor to determine yaw.


<img width="403" height="276" alt="image" src="https://github.com/user-attachments/assets/dae1d902-8275-411a-806a-0e9d4ff7478a" />


<img width="602" height="217" alt="image" src="https://github.com/user-attachments/assets/048ff12e-6652-40c7-b886-49e22c17a0a7" />


## Conclusion
To conclude, as stated in the report:
The developed robot achieved specified requirements, outperforming previous jumping robots in class, that utilized a secondary mode of locomotion. It demonstrated a functional jump height of 1.5 meters and reached a maximum speed of 2.11 m/s, with capacity for carrying payloads. Additionally, the robot effectively cleared obstacles through its dynamic jumping capability. Custom-designed TPU wheels were integrated to endure 500 cycles and effectively absorb the shock from landing impacts. All components were efficiently controlled via a wireless controller using 2.4 GHz communication. However, further refinement and development are necessary to enhance the robot's agility and optimize it for field applications.
