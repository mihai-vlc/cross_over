==== CROSS OVER ====
    Author: Mihai Ionut Vilcu 
    Date: March-April 2013
    Made with: Visual Studio 2010

==== PREVIEW ====
    http://www.youtube.com/watch?v=Cw2pjUyZqqw

==== Levels Documentation ====

    the levels are hosted in CO_LEVELS a folder in the current folder of the exe
    you can make your own levels and share them with your friends using the level creator

== About the game & levels ==

    By editing the levels you can add obstacles for the player.
    The objective of the game is to get on the right site to the gate. 
    You are the purple square.
    When the player collides with any obstacle his position is set back to the start.

    
== Level Creator == 
    
    You can use the Level creator as a GUI for generating the txt files.
    To add the objects click and drag then from the top left corner.
    One you reached the desired position let go on the mouse button.
    You can resize and move the objects around.
    You can change their settings from the top right corner( make sure you have the object selected).
    Once you are happy with the resut you can save it.
    The level name must follow this pattern: level1.txt, level2.txt, level3.txt
    You can add as many levels as you want the game will automatically detect when it reached the finish.
    The levels should be placed in the `CO_LEVELS`.
    You can also open a level and change it using the Open option.
    The test button will allow you to test a level before saving it.
    You can select objects and move/resize/delete/change their properties.
    You can use the options cut/copy/paste for single objects from the right click menu and also from keyboard(CTRL+x, CTRL+c, CTRL+v).
    You can select objects and move them around with the mouse and with the keyboard also using the UP/DOWN/LEFT/RIGHT arrows. 
    If you hold shift down you will move them with the step 10.
    
    
== Container ==
    Container size is 785x375
        width: 785
        height: 375

== Move Type ==
    1 - up and down
    2|30 - circle with radius 30
    3 - bouncing on the walls
    4 - Follows the player
    5 - left and right

== Objects ==
    The object are added in the container(represented by the middle rectangle).
    Each object should be added on a new line in the txt file.
    So the 0,0 coordinates of the objects is represented by the top left of the container.
    Here is the list of the object you can add in the game and how to use it:

    _______________________________________
    
    Rectangle(x, y, width, height, move_type, speed)

    Arguments: x - top left x coordinate (integer)
               y - top left y coordinate (integer)
               width - width of the object (integer)
               height - height of the object (integer)
               move_type - the type of the move (more details about it's possible values in the Move type area ~line 19)
               speed - the speed of the object (it's values may depend on the type of movement)

    Example: Rectangle(10,10,200,200,1,10)
    
    _______________________________________
    
    Ellipse(x, y, width, height, move_type, speed)    
    
    Arguments: x - top left x coordinate (integer)
               y - top left y coordinate (integer)
               width - width of the object (integer)
               height - height of the object (integer)
               move_type - the type of the move (more details about it's possible values in the Move type area ~line 19)
               speed - the speed of the object (it's values may depend on the type of movement)

    Example: Ellipse(10,10,200,200,2|50,10)

    _______________________________________
    
    Wall(x, y, width, height, type)    
    
    Arguments: x - top left x coordinate (integer)
               y - top left y coordinate (integer)
               width - width of the object (integer)
               height - height of the object (integer)
               move_type - the type of the move (more details about it's possible values in the Move type area ~line 19)
               type - 1 = Rectangle, 2 = Ellipse

    Example: Wall(40,80,200,200,1)

   
    
    
    
    
    
    
    