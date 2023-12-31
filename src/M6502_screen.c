#include "M6502.h"
#include "M6502_screen.h"
#include "M6502_flags.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ppu.h"
#include "hardware_interrupts.h"

ushort16_t global_pc = 0;
uchar8_t global_opcode = 0;
uchar8_t frame = 0;

void draw_screen(struct M6502* computer, ushort16_t program_size){

    // SDL window title
    const char* title = "fTNES Emu";

    // SDL init
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    // SDL types
    SDL_Event event;
    SDL_Window* window;
    SDL_Rect bounds;
    SDL_Rect pixel;
    SDL_GetDisplayBounds(1, &bounds); // Get bounds of the first or second display

    //window = SDL_CreateWindow(title, display_bounds.x+300, display_bounds.y+300, 256*2, 240*2, 0); // other monitor
    window = SDL_CreateWindow(title, bounds.x+1600, bounds.y+600, 256*2, 240*2, SDL_WINDOW_ALWAYS_ON_TOP);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create renderer for specific window

    bool running = true;

    // Init for drawing
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // set initial bg color
    SDL_RenderClear(renderer); // clear current target with bg color
    SDL_Color px_color = {255, 255, 255, 255}; //set initial sprite color
    SDL_SetRenderDrawColor(renderer, px_color.r, px_color.g, px_color.b, px_color.a); // set pixel color

    // setup pixel start
    int start_x = 0; // Start x position
    int start_y = 0; // Start y position
    int pixel_size = 1; // pixel sqaure size
    pixel.x = start_x;
    pixel.y = start_y;
    pixel.w = pixel_size;
    pixel.h = pixel_size;

    // init vars for loop
    int scanline_x = 0;
    int scanline_y = 0;
    uchar8_t** virtual_screen = NULL; // 2D array virtual screen

    // test stuff
    int temp = 0; // temp < 1, for one iteration of while loop
    int v_ready = 0;
    int first_vblank = 0;

    // event loop while program running 
    while(running){
        while(SDL_PollEvent(&event)){if(event.type == SDL_QUIT) running = false;}

        for(int e = 0; e < 1; e++)
            execute_instructions(computer, program_size);

        if(first_vblank == 2){v_ready = 1;} // giving moment to fill oam

        if(scanline_y == 0){clear_bit(7, &PPU_status);} // clear vblank bit

        for(scanline_x = 0; scanline_x < 256; scanline_x++){
            if(scanline_y <  240){
                //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                if(v_ready == 1){
                    if(virtual_screen[scanline_y][scanline_x] == 1){ SDL_RenderFillRect(renderer, &pixel); }
                    pixel.x += pixel_size; // not sure why this works before and after rect 
                }
            }
            if(scanline_x == 255){ pixel.y += pixel_size; pixel.x = 0; }
        } scanline_y++;
        SDL_RenderPresent(renderer); // presents render
        if(scanline_y == 240){ non_maskable_interrupt(computer); set_bit(7, &PPU_status);} // start vblank bit, call nmi
        if(scanline_y == 260){ // end vblank and frame
            frame ^= 1; pixel.y = 0; scanline_y = 0; first_vblank += 1; // flop frame, reset y, unlock oam v ready
            free(virtual_screen); virtual_screen = parse_oam(computer); temp += 1; } // store oam updates into virtual screen
    }
    // cleanup
    free(virtual_screen); // outside loop
    SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

// x- 512/8=64 || y - 520/8=65 480/8=60 - note vblank is 241-260
// converts input to 8 bit binary - allocates 9 bytes of memory 
char8_t* convert_to_binary(uchar8_t input_n){
    char8_t* binary = (char8_t*)malloc(9); // 8 bits + null terminator
    if(!binary) return NULL; // Check if memory created

    for (char8_t i = 7; i >= 0; i--)
        binary[7 - i] = ((input_n >> i) & 1) + '0';
    binary[8] = '\0'; // Add Null terminator

    return binary;
}


// // below draws CPU info
// void draw_screen(struct M6502* computer, ushort16_t program_size){

//     const char* title = "M6502 Emulator";

//     SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
//     TTF_Init();

//     SDL_Event event;
//     SDL_Window* window;

//     // Text constants - font, size, and color
//     TTF_Font* font = TTF_OpenFont("./font/DejaVuSans.ttf", 21);
//     SDL_Color text_color = {255, 255, 255, 255};

//     // Dev setup to display on specific window, ignore later - 3840x2160
//     SDL_Rect display_bounds;
//     SDL_GetDisplayBounds(1, &display_bounds); // Get bounds of the first or second display

//     //window = SDL_CreateWindow(title, display_bounds.x+300, display_bounds.y+300, 256*2, 240*2, 0); // other monitor
//     window = SDL_CreateWindow(title, display_bounds.x+1600, display_bounds.y+600, 256*2, 240*2, SDL_WINDOW_ALWAYS_ON_TOP);
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create renderer for specific window

//     // Inititializing
//     ushort16_t initial_program_counter = program_counter;
//     bool running = true;
//     int start_x = 75; // Start x position
//     int start_y = 75; // Start y position
//     int spacing = 25; // Space between lines
//     char cpu_register_info[6][20]; // 6 lines that can have up to 20 characters - 2D string array
//     SDL_SetRenderDrawColor(renderer, 0, 100, 128, 255); // sets color

//     // event loop while program running
//     while(running){
//         while(SDL_PollEvent(&event))
//             if(event.type == SDL_QUIT) running = false;
        
//         // Convert status to binary
//         char8_t* binary_status_register = convert_to_binary(status_register);

//         // setting each line or index to a different cpu info
//         sprintf(cpu_register_info[0], "PC: $%04X", global_pc);
//         sprintf(cpu_register_info[1], "Opcode: 0x%02X", global_opcode);
//         sprintf(cpu_register_info[2], "Accumulator: %02X", accumulator);
//         sprintf(cpu_register_info[3], "Stack: %02X", CPU_address[stack_pointer]);
//         sprintf(cpu_register_info[4], "X: $%02X Y: %02X", x_register, y_register);
//         sprintf(cpu_register_info[5], "SR: %s", binary_status_register);
//         free(binary_status_register), binary_status_register = NULL;

//         SDL_RenderClear(renderer); // clear current target with draw color
        
//         // check on this if statement, could be while? remove inits from running loop
//         if(program_counter - initial_program_counter < program_size){
//             for(int i = 0; i < 6; i++){
//                 // Write on a surce and convert to a texture - use rect to position each line
//                 SDL_Surface* text_surface = TTF_RenderText_Solid(font, cpu_register_info[i], text_color);
//                 SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
//                 SDL_Rect text_rect = {start_x, start_y + i * spacing, text_surface->w, text_surface->h};

//                 // copy text to render target - basically stores each line
//                 SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

//                 // Cleanup from new creation
//                 SDL_FreeSurface(text_surface);
//                 SDL_DestroyTexture(text_texture);
//             }

//             SDL_RenderPresent(renderer); // presents render
//             execute_instructions(computer, program_size);
//         }
//         SDL_Delay(100); // .10 seconds (n milliseconds)
//     }

//     // cleanup
//     TTF_CloseFont(font);
//     SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
//     SDL_DestroyWindow(window);
//     TTF_Quit();
//     SDL_Quit();

// }