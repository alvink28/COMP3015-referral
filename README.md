# COMP3015-referral

Visual Studio 2019 16.9.2 and a Windows 10 machine were used to write this code.

#### How does the user interact with your executable? How do you open and control the software you wrote (exe file)?

Please open the executable in the Project_template folder. Opening the project shows some models of a trophy with different PBR on them. On the back row from left to right: Gold, copper, aluminum, titanium, silver.

<img src="https://github.com/alvink28/COMP3015-referral/trophy.JPG" alt="trophy" width="300"/>

#### How does the program code work? How do the classes and functions fit together and who does what?

The program reders a trophy obj. The lights moves around the objects to show the reflection better. Here are the main function in the scenebasic_uniform.cpp.\
-initScene: setup the light position and noise buffers.\
-Compile: linked the vertex shader and fragment shader.\
-Update: rotate the light position around the scene.\
-render: call drawScene.\
-drawScene: call drawFloor and setup the location of the trophys with different roughnes(Gold, copper, aluminum, titanium, silver)\
-drawFloor: It draws the plane that the trophys on

#### What makes your shader program special and how does it compare to similar things? (Where did you get the idea from? What did you start with? How did you make yours unique? Did you start with a given shader/project?)

My shader program uses Physically-based reflection and an attempt on noise rendering. PBR makes the trophys look realistic with the metal finish on it. 
With different roughness settings, we can see the ability of PBR to make metal finishing look better. I used the tutorials on the DLE, combined different techniques and render the trophy model that I found online.

#### link to a video report

https://youtu.be/d4RkcyvJEKk
