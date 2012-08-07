newaction {
  trigger = 'clean',
  description = 'Cleans up the project.',
  shortname = "clean",
  
  execute = function()
    os.rmdir("bin")
    os.rmdir("build")
  end
}

solution "windows-console-graphics"
  configurations { "debug", "release" }
  platforms { "x32", "x64" }

  location "build"
  targetdir "bin"
  
  flags { "FatalWarnings", "ExtraWarnings" }
  
  configuration { "windows" }
    defines { "WIN32", "_WIN32" }
    defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE" }
    
  configuration { "debug" }
    defines { "DEBUG" }
    flags { "Symbols" }

  configuration { "release" }
    defines { "NDEBUG" }
    flags { "Optimize" }
  
  project "SampleApp"
    kind "ConsoleApp"
    language "C++"
    
    links "WinConGfx"
    
    includedirs { "src" }
    files { "src/SampleApp/**.cpp",
            "src/SampleApp/**.hpp" }
  
  project "WinConGfx"
    kind "SharedLib"
    language "C++"
    
    defines "WINCONGFX_DLL"
    
    includedirs { "src" }
    files { "src/WinConGfx/**.cpp",
            "src/WinConGfx/**.hpp" }