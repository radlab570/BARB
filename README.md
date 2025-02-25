# Unreal Implementation
This article describes the methodology for adding the provided C++ files into Unreal Engine 5.5 using Visual Studio 2022.  This will allow the user to receive the position and rotation data of the phone and copy the phone's movement in Unreal.

## Prerequisites
### Unreal Engine 5.5
 1. Download and install the [Epic Games Launcher](https://store.epicgames.com/en-US/download)
 2. Open the Launcher and create an account if necessary.
 3. Install the Unreal Engine
### Visual Studio 2022
 1. Install [Microsoft Visual Studio](https://visualstudio.microsoft.com/)
 2. Once installed, open Visual Studio and click on ***tools*** on the top of the page.
 3. Click ***Get Tools and Features***
 4. When the Visual Studio Installer window appears, click ***Modify***.
 5. Check ***Game development with C++*** under **Gaming**.
 6. Click ***Install***.


## Steps For Implementing BARB In Unreal
1. Open or create a project. 
> **Note:** The necessary files in this repo are C++ files, but you can add C++ files to a blueprint project.
2. In the Outliner, click on the ***PlayerStart*** instance.
3. Under the **Details** tab of **PlayerStart**, click on the ***+Add*** button.
4. In the resulting dropdown menu, click on ***New C++ component***.
5. In the new window, click ***Actor Component***, then the ***Next*** button.
6. In the **Name** field, change the name to ```CameraActorComponent```.  Then click ***Create Class***.
7. Wait for Unreal to create the files and open Visual Studio.
8. At this point you can add to code from this repo to project in two different ways.

   - Option 1: **Copy** the code from ```CameraActorComponent.cpp``` and ```CameraActorComponent.h``` to the corresponding files just created in your project.
   - Option 2: **Replace** ```CameraActorComponent.cpp``` and ```CameraActorComponent.h``` located in ```...\Unreal Projects\<Project Name>\Source\<Project Name>```  with the corresponding files in this repo.
9. Save the ```CameraActorComponent.cpp``` and ```CameraActorComponent.h``` files.
10. In Visual Studio, open ```...\Unreal Projects\<Project Name>\Source\<Project Name>\<Project Name>.Build.cs```.
11. Edit the ```PublicDependencyModuleNames``` so that ```"Sockets"``` and ```"Networking"``` are added to the list:
```C#
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Sockets", "Networking" });
```
12. Save ```\<Project Name>.Build.cs```
13. **Switch back to the Unreal Engine** and compile the C++ and C# code with ***Ctrl+Alt+F11***.
14. In Unreal, towards the bottom of the screen, click on the **Content Drawer** button.
15. On the left side of the **Content Drawer** window, open the ```\<Project Name>``` dropdown menu.  (This should already be open.)
16. Dropdown the ***All*** folder, then dropdown the ***C++ Classes*** folder, then click on the ```\<Project Name>``` folder.
17. To the right of the folders, you should see the **CameraActorComponent** C++ class.
18. Drag the ***CameraActorComponent*** C++ class to the ***PlayerStart*** instance.
19. Save the project in Unreal.
20. Click the green play button to run the level.
21. Run the **Google ARCore** app on your phone.  The information from the phone will be sent to Unreal, and the **PlayerStart** will move around and rotate as the phone does.


# Android Phone Implementation
1. Install Android Studio (Windows install)
2. Install Google ARCore (git clone operation)
3. Install Google Pixel USB drivers from within Android Studio
   - tools -> sdk manager -> android sdk ... make sure to check "usb debugging"
4. Within the Android project, you have to make the following addition to androidmanifest.xml:
   - <manifest \xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    package="com.google.ar.core.examples.java.helloar">
