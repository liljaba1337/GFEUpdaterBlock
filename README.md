# Nvidia GeForce Experience Update Notification Removal Tool  

## What is this for?  
The new GeForce Experience update isn't great, so many people prefer staying on the last good version. However, it constantly shows an annoying "new update" notification on every launch. This tool prevents that notification from appearing.  

## How Does This Work?  
GeForce Experience stores a configuration file in its directory, which contains a path to its updater. Removing that path prevents it from accessing the installer, and therefore, stops it from prompting you to update.  

---

# Installation  

## Manual  
If you don’t want to use this tool, you can patch GeForce Experience manually.  

### Easy way:  
Using the `-skipInstallerLaunch` flag with `NVIDIA GeForce Experience.exe` makes it skip the update check. You can add this flag to your Start menu or desktop shortcut.  

**Drawback:** If you launch GeForce Experience from the system tray (like I do), this method won’t work.  

### Hard way:  
You can edit the configuration file yourself:  
1. Navigate to:  
   ```
   C:\Program Files\NVIDIA Corporation\NVIDIA GeForce Experience\NVIDIA GeForce Experience.json
   ```  
2. Open the file in a text editor.  
3. Find and delete this line:  
   ```
   "nv-self-update-path=Downloader\\gfeupdate.json",
   ```  
4. Save the file, and you're done.  

## Automatic  
Download the [latest release](https://github.com/liljaba1337/GFEUpdaterBlock/releases/latest), run it, and that’s it. The tool will handle everything for you.  

---

# Q&A  

### Why does this require administrator privileges?  
The GeForce Experience folder is protected, so modifying files inside it requires administrator permissions.  

### I don't like how it works. How do I revert everything?  
The software created a backup file next to the original json one. Just delete `NVIDIA GeForce Experience.json` and rename `NVIDIA GeForce Experience.json.backup` to `NVIDIA GeForce Experience.json`.

---

## Need More Help?  
If you run into any issues, open an **issue** on the project's repository.  
I'll be happy to help :)
