#include "SDLGraphicsProgram.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h):m_screenWidth(w),m_screenHeight(h) {
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	m_window = NULL;
	// Render flag

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	} else {
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		// We want to request a double buffer for smooth updating.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		m_window = SDL_CreateWindow( "Visualizer",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                m_screenWidth,
                                m_screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		// Check if Window did not create.
		if( m_window == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		m_openGLContext = SDL_GL_CreateContext( m_window );
		if( m_openGLContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		// Initialize GLAD Library
		if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
			errorStream << "Failed to iniitalize GLAD\n";
			success = false;
		}

		//Initialize OpenGL
		if(!InitGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}
  	}

    // If initialization did not work, then print out a list of errors in the constructor.
    if(!success) {
        errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
        std::string errors=errorStream.str();
        SDL_Log("%s\n",errors.c_str());
    }else{
        SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
    }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	GetOpenGLVersionInfo();

}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    //Destroy window
	SDL_DestroyWindow( m_window );
	// Point m_window to NULL to ensure it points to nothing.
	m_window = nullptr;
	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL() {
	//Success flag
	bool success = true;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //// Parse models passed in on command line
    //ObjModelParser parser;
    //for(int i = 0; models[i]; i++) {
        //Mesh* m = parser.parse(models[i]);
        //if(m == nullptr) {
            //std::cerr << "Invalid object file" << std::endl;
            //return false;
        //}
        //Object* o = new Object(m);
		//o->MakeBuffers();
    //}

	return success;
}


// Update OpenGL
void SDLGraphicsProgram::Update(){
    // Here we hard-code a giant scene
    // Yuck, we'll fix this in a future assignment.
    //ObjectManager::Instance().GetObject(0).GetTransform().LoadIdentity();
    // Push back our wall a bit
    //ObjectManager::Instance().GetObject(0).GetTransform().Translate(0.0f,0.0f,-8.0f);

    // Update all of the objects
    //ObjectManager::Instance().UpdateAll(m_screenWidth,m_screenHeight);
    v->update();
    
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::Render(){
	// Setup our OpenGL State machine
    // TODO: Read this
    // The below command is new!

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    // TODO: Read this
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Nice way to debug your scene in wireframe!
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Render all of our objects in a simple loop
    //ObjectManager::Instance().RenderAll();

	// Delay to slow things down just a bit!
    SDL_Delay(20); 
}


//Loops forever!
void SDLGraphicsProgram::Loop(void (*loopCallback)(void)){
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set the camera speed for how fast we move.
    float cameraSpeed = 0.50f;

    bool wKey = false, wKeyLift = true;

    // While application is running
    while(!quit){
     	     	 //Handle events on queue
		while(SDL_PollEvent( &e ) != 0){
        	// User posts an event to quit
	        // An example is hitting the "x" in the corner of the window.
    	    if(e.type == SDL_QUIT){
        		quit = true;
	        }
            // Handle keyboard input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                //Camera::Instance().MouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_LEFT:
                            //Camera::Instance().MoveLeft(cameraSpeed);
                            break;
                        case SDLK_RIGHT:
                            //Camera::Instance().MoveRight(cameraSpeed);
                            break;
                        case SDLK_UP:
                            //Camera::Instance().MoveForward(cameraSpeed);
                            break;
                        case SDLK_DOWN:
                            //Camera::Instance().MoveBackward(cameraSpeed);
                            break;
                        case SDLK_RSHIFT:
                            //Camera::Instance().MoveUp(cameraSpeed);
                            break;
                        case SDLK_RCTRL:
                            //Camera::Instance().MoveDown(cameraSpeed);
                            break;
                        case SDLK_q:
                            // Quit on 'q' press
                            return;
                        case SDLK_w:
                            //Switch to wireframe
                            if(wKeyLift) {
                                wKey = !wKey;
                                wKeyLift = false;
                            }
                            if(wKey) {
                                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                            } else {
                                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                            }
                            break;
                    }
                    break;
                case SDL_KEYUP:
                  switch(e.key.keysym.sym) {
                      case SDLK_w:
                          //Switch back to fill
                          wKeyLift = true;
                  }
            }
      	} // End SDL_PollEvent loop.
		
		// Update our scene
		Update();
		// Render using OpenGL
	    Render(); 	// TODO: potentially move this depending on your logic
        loopCallback();
					// for how you handle drawing a triangle or rectangle.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(GetSDLWindow());
	}
    //Disable text input
    SDL_StopTextInput();
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow(){
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
