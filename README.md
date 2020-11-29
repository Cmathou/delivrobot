# Projet Prototype_ groupe 6

**Nom:** Deliv'Robot

**Auteurs:** BRIAND Estelle _ DUPUIS Florian _ KAHAN Jérémie _ MATHOU Cédric

**Ecole:** CPE Lyon

**Lien video:** https://www.youtube.com/watch?v=jA0dbAfiRVc 

## But du projet:
Projet de prototypage libre sur 20h

**Problématique:** Comment délivrer un colis sans se déplacer et s'assurer qu'il s'ouvre qu'en présence d'une personne ?​ 

**Solution:** un robot télécommandé apportant une boite qui ne s'ouvre qu'en présence d'une personne davant son capteur

![synoptique image](synoptique.png)

## Description git

        /app =  contient tout les fichiers pour l'application android

        /doc = document tel que le fichier .stl

        /[PC]_deliv_robot_ws => worspace ROS executable sur PC linux

        /[raspberry]_deliv_robot_ws => worspace ROS executable sur la raspberry
## Mise en place
**Prerequis:**
- Arduino IDE
- ROS melodic [PC] ou ROS kinetic [raspberry]
- app inventor applocation
- telephone avec bluetooth Low Energy
- carte arduino UNO, ESP32 et raspberry
- base roulante avec 3 roue "omniwheels"
- moteur dynamixel

--> Télécharger le répertoire gitlab

--> Televerser les differents scripts pour les arduino et ESP32*

*Pour les ESP32, dans Arsuino IDE il faut pouvoir se connecter à une ESP32 et pour se faire, suivez le tutoreil :
https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
--> Cabler les capteurs sur l'ESP32
--> Copier le dossier [raspberry]_deliv_robot_ws sur la raspberry
--> Brancher sur la raspberry:
        - arduino UNO
        - dynamixel
        - ESP32
--> Telecharger l'application .apk sur votre téléphone
--> executer start_project.sh sur la raspberry
--> Connecter le téléphone au bluetooth via l'application
--> Profiter

### Moteur dynamixel
-	Telecharger le git : https://github.com/arebgun/dynamixel_motor.git
-	Sur la VM autoriser l’USB Xevelabs
-	Sur un teminal, pour savoir quel port correspond au moteur : ```$ls /dev/```

On verra un ttyAMC0 ```$ sudo chmod 666 /dev/ttyAMC0```

Ensuite ```$ rosrun dynamixel_driver info_dump.py -p /dev/ttyAMC0 1```

Pour faire tourner le motor pas en continue :  

trouver où se trouve le set_servo_config.py (logiquement dans dynamixel_driver)
```$ ./set_servo_config.py –port=/dev/ttyAMC0 –baud=1000000 –ccw-angle=1023 –cw-angle-limit=0 1```

### ROSSERIAL
Installer la librairie Arduino IDE rosserial à l'aide du gestionnaire de blibliothèque


### Comment exécuter le code principal (PC ou raspberry)
- Installer tout le dossier [PC]_deliv_robot_ws
- Installer rosserial(https://github.com/ros-drivers/rosserial.git) et dynamixel_motor(https://github.com/arebgun/dynamixel_motor.git ) dans [PC]_deliv_robot_ws/src
- ```$ cd [PC]_deliv_robot_ws```
- ```$ catkin_make```
- ```$ src/deliv_robot/start_project.sh```, ce script va s'occuper de donner les droits de lecture pour les ports séries et d'executer tout les noeuds ROS

