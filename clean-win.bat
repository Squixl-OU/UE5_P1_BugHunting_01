@echo off

del /S /Q *.sln
del /S /Q *.vcxproj
del /S /Q *.vcxproj.filters
del /S /Q *.vcxproj.user
rmdir /S /Q Build
rmdir /S /Q Binaries
rmdir /S /Q Saved\Autosaves
rmdir /S /Q Saved\Backup
rmdir /S /Q Saved\Collections
rmdir /S /Q Saved\Logs
rmdir /S /Q DerivedDataCache
rmdir /S /Q Intermediate

pause
