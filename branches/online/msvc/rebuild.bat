del *.obj
del *.pdb
set INCLUDE=C:\Program Files\Microsoft Visual Studio .NET\Vc7\include;C:\Program Files\Microsoft Visual Studio .NET\Vc7\atlmfc\include;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\include\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\include;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\include
set LIB=C:\Program Files\Microsoft Visual Studio .NET\Vc7\lib;C:\Program Files\Microsoft Visual Studio .NET\Vc7\atlmfc\lib;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\lib\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\lib;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\lib
set _ACP_ATLPROV=C:\Program Files\Microsoft Visual Studio .NET\Vc7\bin\ATLPROV.DLL
set _ACP_INCLUDE=C:\Projects\filter\include;"C:\Lemur\lemur-1.9.2\utility\include";"C:\Lemur\lemur-1.9.2\retrieval\include";"C:\Lemur\lemur-1.9.2\index\include";C:\cygwin\Jama;C:\cygwin\TNT;C:\Program Files\Microsoft Visual Studio .NET\Vc7\include;C:\Program Files\Microsoft Visual Studio .NET\Vc7\atlmfc\include;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\include\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\include;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\include
set _ACP_LIB=C:\Program Files\Microsoft Visual Studio .NET\Vc7\lib;C:\Program Files\Microsoft Visual Studio .NET\Vc7\atlmfc\lib;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\lib\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\lib;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\lib;C:\Projects\filter\include;"C:\Lemur\lemur-1.9.2\utility\include";"C:\Lemur\lemur-1.9.2\retrieval\include";"C:\Lemur\lemur-1.9.2\index\include";C:\cygwin\Jama;C:\cygwin\TNT;C:\Program Files\Microsoft Visual Studio .NET\Vc7\include;C:\Program Files\Microsoft Visual Studio .NET\Vc7\atlmfc\include;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\include\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Vc7\PlatformSDK\include;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\include
set _ACP_PATH=C:\Program Files\Microsoft Visual Studio .NET\Vc7\bin;C:\Program Files\Microsoft Visual Studio .NET\Common7\Tools\bin\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Common7\Tools\bin;C:\Program Files\Microsoft Visual Studio .NET\Common7\tools;C:\Program Files\Microsoft Visual Studio .NET\Common7\ide;C:\Program Files\HTML Help Workshop\;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\bin;C:\WINDOWS\Microsoft.NET\Framework\v1.0.3705;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\Common Files\Adaptec Shared\System
Path=C:\Program Files\Microsoft Visual Studio .NET\Vc7\bin;C:\Program Files\Microsoft Visual Studio .NET\Common7\Tools\bin\prerelease;C:\Program Files\Microsoft Visual Studio .NET\Common7\Tools\bin;C:\Program Files\Microsoft Visual Studio .NET\Common7\tools;C:\Program Files\Microsoft Visual Studio .NET\Common7\ide;C:\Program Files\HTML Help Workshop\;C:\Program Files\Microsoft Visual Studio .NET\FrameworkSDK\bin;C:\WINDOWS\Microsoft.NET\Framework\v1.0.3705;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\Program Files\Common Files\Adaptec Shared\System
cl.exe @bbrtrain.compile.rsp /nologo
link.exe @bbrtrain.link.rsp
cl.exe @bbrclassify.compile.rsp /nologo
link.exe @bbrclassify.link.rsp
