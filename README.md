# How to set up Visual Studio (miniimum required version - 15(2017))

## We need to tell the compiler where to find the project headers (.hpp files)

- open .sln file
- Project -> Propeties
- Configuretion - All Configurations
- C/C++ -> General
- In field "Additional Include Directories" put this text - 
```
$(SolutionDir)engine\utils;
$(SolutionDir)interface\utils;
$(SolutionDir)SFML-2.5.1\include;
$(SolutionDir)engine\include;
$(SolutionDir)interface\include;
%(AdditionalIncludeDirectories)
```
![](https://raw.githubusercontent.com/sbudilko/my_chess/master/1.png)

## Add SFML_STATIC  macro in the preprocessor options of project

- Project -> Propeties
- Configuretion - Release
- C/C++ -> Preprocessor
- In field "Preprocessor Definitions" put this text - `SFML_STATIC;`

![](https://raw.githubusercontent.com/sbudilko/my_chess/master/2.png)

## We need to tell the compiler where to find the project libraries
- Project -> Propeties
- Linker -> General
- Configuretion - All Configurations
- In field "Additional Libraries Directories" put this text - `$(SolutionDir)SFML-2.5.1\include;`

![](https://raw.githubusercontent.com/sbudilko/my_chess/master/3.png)

## The last step is to link your application to the SFML libraries
  ### Dinamic libraries for Debug configuration
  - Project -> Propeties
  - Linker -> Input
  - Configuretion - Debug
  - In field "Additional Dependencies" put this text - 
   ```
   sfml-graphics-d.lib
   sfml-window-d.lib
   sfml-audio-d.lib
   sfml-system-d.lib
   ```
   
   ![](https://raw.githubusercontent.com/sbudilko/my_chess/master/4.png)
    
   ### Static ibraries fo Release configuration
   - Project -> Propeties
   - Linker -> Input
   - Configuretion - Release
   - In field "Additional Dependencies" put this text - 
   ```
   sfml-graphics-s.lib
   sfml-window-s.lib
   sfml-system-s.lib
   sfml-audio-s.lib
   opengl32.lib
   winmm.lib
   gdi32.lib
   freetype.lib
   openal32.lib
   flac.lib
   vorbisenc.lib
   vorbisfile.lib
   vorbis.lib
   ogg.lib
   ```
   ![](https://raw.githubusercontent.com/sbudilko/my_chess/master/5.png)
