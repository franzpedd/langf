# This file is only here to manage my project setup dependencies, langf should be used as a header + langf_platform.c

# standart
import os;
import platform;
import shutil;
import subprocess;
import sys;
# thirdparty
import requests;
import tqdm;

class Util :

    @classmethod
    def Download(itslf, url, path) :

        if(os.path.isfile(path) is True) :
            print(f"{path} already downloaded, skipping...");
            return;
        
        request = requests.get(url, stream=True);
        percentage = int(request.headers.get("content-lenght", 0));
        
        with open(path, "wb") as file, tqdm.tqdm(desc = path, total = percentage, unit = "iB", unit_scale = True, unit_divisor = 1024) as bar :
            for data in request.iter_content(chunk_size = 1024) :
                size = file.write(data);
                bar.update(size);
    
    @classmethod
    def Clone(itself, url, folder, version) :
        
        if(os.path.isdir("Thirdparty") is False) :
            print("Creating Thirdparty folder");
            os.makedirs("Thirdparty");

        if(os.path.isdir(f"Thirdparty/{folder}") is True) :
            print(f"{folder} already exists, skipping...");
            return;
        
        scriptDir = os.getcwd();
        os.chdir("Thirdparty");
        subprocess.call(f"git clone {url} {folder}", shell = True);
        os.chdir(folder);
        subprocess.call(f"git checkout {version}", shell = True);
        os.chdir(scriptDir);

    @classmethod
    def Decompress(itself, file, path) :

        if(os.path.isfile(file) is None) :
            print(f"Decompress Error: {file} doesnt exists, skipping...");
            return;
        
        shutil.unpack_archive(file, path);

class Premake :

    def Download(version) :

        if(os.path.isdir("Thirdparty/premake") is False) :
            os.makedirs("Thirdparty/premake");
        
        url = "";
        path = "";

        if(platform.system() == "Linux") :
            url = f"https://github.com/premake/premake-core/releases/download/v{version}/premake-{version}-linux.tar.gz";
            path = f"Thirdparty/premake/premake-{version}-linux.tar.gz";

        if(platform.system() == "Windows") :
            url = f"https://github.com/premake/premake-core/releases/download/v{version}/premake-{version}-windows.zip";
            path = f"Thirdparty/premake/premake-{version}-windows.zip";

        Util.Download(url, path);
        Util.Decompress(path, "Thirdparty/premake");

    @classmethod
    def Generate(itself) :

        if(len(sys.argv) == 2) :
            subprocess.call(f"Thirdparty/premake/premake5 {sys.argv[1]}", shell = True);
            return;

        if(platform.system() == "Linux") : 
            subprocess.call(f"Thirdparty/premake/premake5 gmake2", shell = True);

        if(platform.system() == "Windows") :
            subprocess.call(f"Thirdparty/premake/premake5 vs2022");

# main scope
Premake.Download("5.0.0-beta2")
Premake.Generate();