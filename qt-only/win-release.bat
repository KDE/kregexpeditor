md kregexpeditor
cp -r kregexpeditor.exe predefined kregexpeditor
copy q:\3.1.2-mt\lib\qt-mt312.dll kregexpeditor
copy c:\winnt\system32\MSVCRTD.DLL kregexpeditor
copy c:\winnt\system32\MSVCP60D.DLL kregexpeditor
cp -r manual kregexpeditor
c:\zip\zip -r kregexpeditor.zip kregexpeditor
