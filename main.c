// Define _GNU_SOURCE to get the definitions of the *_t types.
#if !defined(_GNU_SOURCE)
    #define _GNU_SOURCE
#endif

#define NO_SVG

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#elif defined(__linux__)
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
#else
    #error "Unsupported platform"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#if defined(INIT_PIECE)
    #undef INIT_PIECE
#endif
#define INIT_PIECE(color, num, _x, _y)                       \
{                                                            \
    app->pieces[color][num].normal_pos.x = _x;               \
    app->pieces[color][num].normal_pos.y = _y;               \
    app->pieces[color][num].is_dead = 0;                     \
    app->pieces[color][num].is_selected = 0;                 \
    app->pieces[color][num].rect = (SDL_Rect) {0, 0, 0, 0};  \
    app->pieces[color][num].selection_time = 0;              \
    app->pieces[color][num].last_selection_time = 100;       \
    app->pieces[color][num].previous_pos.x = _x;             \
    app->pieces[color][num].previous_pos.y = _y;             \
}


#if defined(FPS) || defined(FPMS) || defined(EXPECTED_FRAME_TIME)
    #undef FPS
    #undef FPMS
    #undef EXPECTED_FRAME_TIME
#endif
#define FPS 500.0
#define FPMS (FPS / 1000.0)
// in milliseconds
#define EXPECTED_FRAME_TIME (1.0 / FPMS)

#if defined(PAWN) || defined(KNIGHT) || defined(BISHOP) || defined(ROOK) || defined(QUEEN) || defined(KING)
    #undef PAWN
    #undef KNIGHT
    #undef BISHOP
    #undef ROOK
    #undef QUEEN
    #undef KING
#endif
#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

#if defined(BOARD_BACKGROUND)
    #undef BOARD_BACKGROUND
#endif
#define BOARD_BACKGROUND 20, 20, 20, 255

#if defined(REVERSE_ARR)
    #undef REVERSE_ARR
#endif
#define REVERSE_ARR(arr, size)          \
{                                       \
    typeof((arr)[0]) tmp[size];         \
    for (size_t i = 0; i < size; ++i)   \
    {                                   \
        tmp[i] = (arr)[size - i - 1];   \
    }                                   \
    for (size_t i = 0; i < size; ++i)   \
    {                                   \
        (arr)[i] = tmp[i];              \
    }                                   \
}

#if defined(PIECES_IMG_PATH)
    #undef PIECES_IMG_PATH
#endif
#if defined(NO_SVG)
    #define PIECES_IMG_PATH "./chess_com/pieces/"
#else
    #define PIECES_IMG_PATH "./JohnPablok Cburnett Chess set/SVG No shadow/"
#endif

#if defined(B_PAWN_IMG) || defined(B_KNIGHT_IMG) || defined(B_BISHOP_IMG) || defined(B_ROOK_IMG) || defined(B_QUEEN_IMG) || defined(B_KING_IMG)
    #undef B_PAWN_IMG
    #undef B_KNIGHT_IMG
    #undef B_BISHOP_IMG
    #undef B_ROOK_IMG
    #undef B_QUEEN_IMG
    #undef B_KING_IMG
#endif
#if defined(NO_SVG)
    #define B_PAWN_IMG PIECES_IMG_PATH "bp.png"
    #define B_KNIGHT_IMG PIECES_IMG_PATH "bn.png"
    #define B_BISHOP_IMG PIECES_IMG_PATH "bb.png"
    #define B_ROOK_IMG PIECES_IMG_PATH "br.png"
    #define B_QUEEN_IMG PIECES_IMG_PATH "bq.png"
    #define B_KING_IMG PIECES_IMG_PATH "bk.png"
#else
    #define B_PAWN_IMG PIECES_IMG_PATH "b_pawn_svg_NoShadow.svg"
    #define B_KNIGHT_IMG PIECES_IMG_PATH "b_knight_svg_NoShadow.svg"
    #define B_BISHOP_IMG PIECES_IMG_PATH "b_bishop_svg_NoShadow.svg"
    #define B_ROOK_IMG PIECES_IMG_PATH "b_rook_svg_NoShadow.svg"
    #define B_QUEEN_IMG PIECES_IMG_PATH "b_queen_svg_NoShadow.svg"
    #define B_KING_IMG PIECES_IMG_PATH "b_king_svg_NoShadow.svg"
#endif

#if defined(W_PAWN_IMG) || defined(W_KNIGHT_IMG) || defined(W_BISHOP_IMG) || defined(W_ROOK_IMG) || defined(W_QUEEN_IMG) || defined(W_KING_IMG)
    #undef W_PAWN_IMG
    #undef W_KNIGHT_IMG
    #undef W_BISHOP_IMG
    #undef W_ROOK_IMG
    #undef W_QUEEN_IMG
    #undef W_KING_IMG
#endif
#if defined(NO_SVG)
    #define W_PAWN_IMG PIECES_IMG_PATH "wp.png"
    #define W_KNIGHT_IMG PIECES_IMG_PATH "wn.png"
    #define W_BISHOP_IMG PIECES_IMG_PATH "wb.png"
    #define W_ROOK_IMG PIECES_IMG_PATH "wr.png"
    #define W_QUEEN_IMG PIECES_IMG_PATH "wq.png"
    #define W_KING_IMG PIECES_IMG_PATH "wk.png"
#else
    #define W_PAWN_IMG PIECES_IMG_PATH "w_pawn_svg_NoShadow.svg"
    #define W_KNIGHT_IMG PIECES_IMG_PATH "w_knight_svg_NoShadow.svg"
    #define W_BISHOP_IMG PIECES_IMG_PATH "w_bishop_svg_NoShadow.svg"
    #define W_ROOK_IMG PIECES_IMG_PATH "w_rook_svg_NoShadow.svg"
    #define W_QUEEN_IMG PIECES_IMG_PATH "w_queen_svg_NoShadow.svg"
    #define W_KING_IMG PIECES_IMG_PATH "w_king_svg_NoShadow.svg"
#endif

#if defined(HACK_FONT_PATH)
    #undef HACK_FONT_PATH
#endif
#define HACK_FONT_PATH "./Hack/Hack-Regular.ttf"

#if defined(BOARD_FONT) || defined(MENU_FONT_LITTLE) || defined(MENU_FONT_BIG)
    #undef BOARD_FONT
    #undef MENU_FONT_LITTLE
    #undef MENU_FONT_BIG
#endif
#define BOARD_FONT (int) (MIN(app->board.w, app->board.h) / 20)
#define MENU_FONT_LITTLE 16
#define MENU_FONT_BIG 32

#if defined(MAX) || defined(MIN)
    #undef MAX
    #undef MIN
#endif

#define MAX(a, b)           \
({ typeof (a) _a = (a);     \
    typeof (b) _b = (b);    \
    _a > _b ? _a : _b; })

#define MIN(a, b)           \
({ typeof (a) _a = (a);     \
    typeof (b) _b = (b);    \
    _a < _b ? _a : _b; })

#if defined(FLOAT_EQ)
    #undef FLOAT_EQ
#endif
#define FLOAT_EQ(a, b)                          \
({ typeof (a) _a = (a);                         \
    typeof (b) _b = (b);                        \
    _a - _b < 0.01 && _a - _b > -0.01; })

#if defined(ABS)
    #undef ABS
#endif
#define ABS(a)              \
({ typeof (a) _a = (a);     \
    _a < 0 ? -_a : _a; })

typedef struct app_s
{
    struct
    {
        char source_path[512];
        SDL_Surface* surface;
        SDL_Texture* texture;
    } white_pieces_surfaces[6];

    struct
    {
        char source_path[512];
        SDL_Surface* surface;
        SDL_Texture* texture;
    } black_pieces_surfaces[6];

    struct 
    {
        enum 
        {
            WHITE,
            BLACK
        } turn;

        enum
        {
            MENU,
            GAME
        } app_state;

        enum
        {
            GAME_OVER,
            GAME_STARTED,
            GAME_PAUSED,
            GAME_NOT_STARTED
        } game_state;
    } states;

    struct
    {
        double x;
        double y;
        double w;
        double h;
        SDL_Rect squares[8][8];
    } board;

    struct
    {
        union
        {
            // position on the board when not selected
            struct
            {
                int x;
                int y;
            } normal_pos; 

            // position on the screen when selected
            // actually the same as the mouse position, 
            //     except that this represent the center of the piece
            //     and is double to facilitate calculations without having to convert
            struct
            {
                double x;
                double y;
            } selected_pos;
        };
        struct
        {
            int x;
            int y;
        } previous_pos; 
        SDL_Rect rect;
        int is_dead;
        int is_selected;
        // When selected, this is the time spent while selected
        // Usefull for animations
        // In milliseconds
        Uint64 selection_time;
        // When not selected, this is the time since it has been released
        // Usefull for animations after moving
        // In milliseconds
        Uint64 last_selection_time;
    } pieces[2][16];

    struct 
    {
        // buttons[0] = left button
        // buttons[1] = middle button
        // buttons[2] = right button
        struct 
        {
            enum 
            {
                UP,
                DOWN
            } state;
        } buttons[3];
        int x;
        int y;
        int is_selecting;
    } mouse;

    struct 
    {
        SDL_Window *window;
        SDL_Renderer *renderer;
    } resources;
    
    struct 
    {
        int w;
        int h;
    } screen;
    
    struct 
    {
        TTF_Font* ptr;
        size_t size;
    } font;

    enum 
    {
        WHITE_POV,
        BLACK_POV
    } pov;

    Uint64 last_frame_time;

    int freed;

} app_t;

void quit_game(app_t* app)
{
    SDL_DestroyRenderer(app->resources.renderer);
    SDL_DestroyWindow(app->resources.window);
    for (size_t i = 0; i < 6; ++i)
    {
        SDL_FreeSurface(app->white_pieces_surfaces[i].surface);
        SDL_FreeSurface(app->black_pieces_surfaces[i].surface);
        SDL_DestroyTexture(app->white_pieces_surfaces[i].texture);
        SDL_DestroyTexture(app->black_pieces_surfaces[i].texture);
    }
    TTF_CloseFont(app->font.ptr);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    app->freed = 1;
}

void set_pieces_init_pos(app_t* app)
{
    if (!app || app->freed)
    {
        return;
    }

    // Set pawns
    for (size_t i = 0; i < 8; ++i)
    {
        app->pieces[WHITE][i].normal_pos.x = i;
        app->pieces[WHITE][i].previous_pos.x = i;
        if (app->pov == WHITE_POV)
        {
            app->pieces[WHITE][i].normal_pos.y = 6;
            app->pieces[WHITE][i].previous_pos.y = 6;
        }
        else
        {
            app->pieces[WHITE][i].normal_pos.y = 1;
            app->pieces[WHITE][i].previous_pos.y = 1;
        }
        app->pieces[WHITE][i].is_dead = 0;
        app->pieces[WHITE][i].is_selected = 0;
        app->pieces[WHITE][i].rect = (SDL_Rect) {0, 0, 0, 0};
        app->pieces[WHITE][i].selection_time = 0;
        app->pieces[WHITE][i].last_selection_time = 100;

        app->pieces[BLACK][i].normal_pos.x = i;
        app->pieces[BLACK][i].previous_pos.x = i;
        if (app->pov == WHITE_POV)
        {
            app->pieces[BLACK][i].normal_pos.y = 1;
            app->pieces[BLACK][i].previous_pos.y = 1;
        }
        else
        {
            app->pieces[BLACK][i].normal_pos.y = 6;
            app->pieces[BLACK][i].previous_pos.y = 6;
        }
        app->pieces[BLACK][i].is_dead = 0;
        app->pieces[BLACK][i].is_selected = 0;
        app->pieces[BLACK][i].rect = (SDL_Rect) {0, 0, 0, 0};
        app->pieces[BLACK][i].selection_time = 0;
        app->pieces[BLACK][i].last_selection_time = 100;
    }

    // Set knights
    if (app->pov == WHITE_POV)
    {
        INIT_PIECE(WHITE, 8, 1, 7);

        INIT_PIECE(WHITE, 9, 6, 7);

        INIT_PIECE(BLACK, 8, 1, 0);            
        
        INIT_PIECE(BLACK, 9, 6, 0);
    }
    else
    {
        INIT_PIECE(WHITE, 8, 1, 0);

        INIT_PIECE(WHITE, 9, 6, 0);

        INIT_PIECE(BLACK, 8, 1, 7);            
        
        INIT_PIECE(BLACK, 9, 6, 7);
    }

    // Set bishops
    if (app->pov == WHITE_POV)
    {
        INIT_PIECE(WHITE, 10, 2, 7);

        INIT_PIECE(WHITE, 11, 5, 7);

        INIT_PIECE(BLACK, 10, 2, 0);            
        
        INIT_PIECE(BLACK, 11, 5, 0);
    }
    else
    {
        INIT_PIECE(WHITE, 10, 2, 0);

        INIT_PIECE(WHITE, 11, 5, 0);

        INIT_PIECE(BLACK, 10, 2, 7);            
        
        INIT_PIECE(BLACK, 11, 5, 7);
    }

    // Set rooks
    if (app->pov == WHITE_POV)
    {
        INIT_PIECE(WHITE, 12, 0, 7);

        INIT_PIECE(WHITE, 13, 7, 7);

        INIT_PIECE(BLACK, 12, 0, 0);            
        
        INIT_PIECE(BLACK, 13, 7, 0);
    }
    else
    {
        INIT_PIECE(WHITE, 12, 0, 0);

        INIT_PIECE(WHITE, 13, 7, 0);

        INIT_PIECE(BLACK, 12, 0, 7);            
        
        INIT_PIECE(BLACK, 13, 7, 7);
    }

    // Set queens
    if (app->pov == WHITE_POV)
    {
        INIT_PIECE(WHITE, 14, 3, 7);

        INIT_PIECE(BLACK, 14, 3, 0);
    }
    else
    {
        INIT_PIECE(WHITE, 14, 3, 0);

        INIT_PIECE(BLACK, 14, 3, 7);
    }

    // Set kings
    if (app->pov == WHITE_POV)
    {
        INIT_PIECE(WHITE, 15, 4, 7);

        INIT_PIECE(BLACK, 15, 4, 0);
    }
    else
    {
        INIT_PIECE(WHITE, 15, 4, 0);

        INIT_PIECE(BLACK, 15, 4, 7);
    }
}

app_t* init_game(app_t* app)
{
    if (!app)
    {
        app = malloc(sizeof(app_t));
    }
    if (!app)
    {
        return NULL;
    }
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
        goto dealloc;
        return NULL;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        fprintf(stderr, "Failed to init SDL_image: %s\n", IMG_GetError());
        goto dealloc;
        return NULL;
    }
    app->resources.window = SDL_CreateWindow("dumb-chess",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE | SDL_WINDOW_MAXIMIZED);
    if (!app->resources.window)
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        goto dealloc;
        return NULL;
    }
    app->resources.renderer = SDL_CreateRenderer(app->resources.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app->resources.renderer)
    {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        goto dealloc;
        return NULL;
    }
    
    if (SDL_SetRenderDrawColor(app->resources.renderer, BOARD_BACKGROUND))
    {
        fprintf(stderr, "Failed to set render draw color: %s\n", SDL_GetError());
        goto dealloc;
        return NULL;
    }
    if (SDL_RenderClear(app->resources.renderer))
    {
        fprintf(stderr, "Failed to clear renderer: %s\n", SDL_GetError());
        goto dealloc;
        return NULL;
    }
    SDL_RenderPresent(app->resources.renderer);
    app->freed = 0;
    /* if (SDL_CaptureMouse(SDL_TRUE))
    {
        fprintf(stderr, "Failed to capture mouse: %s\n", SDL_GetError());
    } */
    SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
    SDL_GetWindowSize(app->resources.window, &app->screen.w, &app->screen.h);
    app->board.x = 0.0;
    app->board.y = 0.0;
    app->board.w = 0.0;
    app->board.h = 0.0;
    if (TTF_Init())
    {
        fprintf(stderr, "Failed to init TTF: %s\n", TTF_GetError());
        goto dealloc;
        return NULL;
    }
    app->font.ptr = TTF_OpenFont(HACK_FONT_PATH, BOARD_FONT);
    if (!app->font.ptr)
    {
        fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
        goto dealloc;
        return NULL;
    }
    app->font.size = BOARD_FONT;
    app->pov = WHITE_POV;
    for (size_t i = 0; i < 6; ++i)
    {
        memset(app->white_pieces_surfaces[i].source_path, 0, 512);
        memset(app->black_pieces_surfaces[i].source_path, 0, 512);
    }
    strcpy(app->white_pieces_surfaces[0].source_path, W_PAWN_IMG);
    strcpy(app->white_pieces_surfaces[1].source_path, W_KNIGHT_IMG);
    strcpy(app->white_pieces_surfaces[2].source_path, W_BISHOP_IMG);
    strcpy(app->white_pieces_surfaces[3].source_path, W_ROOK_IMG);
    strcpy(app->white_pieces_surfaces[4].source_path, W_QUEEN_IMG);
    strcpy(app->white_pieces_surfaces[5].source_path, W_KING_IMG);
    strcpy(app->black_pieces_surfaces[0].source_path, B_PAWN_IMG);
    strcpy(app->black_pieces_surfaces[1].source_path, B_KNIGHT_IMG);
    strcpy(app->black_pieces_surfaces[2].source_path, B_BISHOP_IMG);
    strcpy(app->black_pieces_surfaces[3].source_path, B_ROOK_IMG);
    strcpy(app->black_pieces_surfaces[4].source_path, B_QUEEN_IMG);
    strcpy(app->black_pieces_surfaces[5].source_path, B_KING_IMG);
    for (size_t i = 0; i < 6; ++i)
    {
        app->white_pieces_surfaces[i].surface = NULL;
        app->black_pieces_surfaces[i].surface = NULL;
        app->white_pieces_surfaces[i].texture = NULL;
        app->black_pieces_surfaces[i].texture = NULL;
    }
    for (size_t i = 0; i < 6; ++i)
    {
        app->white_pieces_surfaces[i].surface = IMG_Load(app->white_pieces_surfaces[i].source_path);
        if (!app->white_pieces_surfaces[i].surface)
        {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            goto dealloc;
            return NULL;
        }
        app->white_pieces_surfaces[i].texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[i].surface);
        if (!app->white_pieces_surfaces[i].texture)
        {
            fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
            goto dealloc;
            return NULL;
        }

        app->black_pieces_surfaces[i].surface = IMG_Load(app->black_pieces_surfaces[i].source_path);
        if (!app->black_pieces_surfaces[i].surface)
        {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            goto dealloc;
            return NULL;
        }
        app->black_pieces_surfaces[i].texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[i].surface);
        if (!app->black_pieces_surfaces[i].texture)
        {
            fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
            goto dealloc;
            return NULL;
        }
    }

    app->states.turn = WHITE;
    app->states.app_state = MENU;
    app->states.game_state = GAME_NOT_STARTED;

    app->mouse.buttons[0].state = UP;
    app->mouse.buttons[1].state = UP;
    app->mouse.buttons[2].state = UP;
    app->mouse.is_selecting = 0;

    app->last_frame_time = 0;

    for (size_t i = 0; i < 8; ++i)
    {
        for (size_t j = 0; j < 8; ++j)
        {
            app->board.squares[i][j].x = 0;
            app->board.squares[i][j].y = 0;
            app->board.squares[i][j].w = 0;
            app->board.squares[i][j].h = 0;
        }
    }

    set_pieces_init_pos(app);
    
    return app;

dealloc:
    quit_game(app);
    return NULL;
}

void change_pov(app_t* app, int pov)
{
    if (app->freed)
    {
        return;
    }
    if (pov == WHITE_POV)
    {
        app->pov = WHITE_POV;
    }
    else if (pov == BLACK_POV)
    {
        app->pov = BLACK_POV;
    }
    // TODO: Change pieces positions if game started, else exit with error
}

void clear_screen(app_t* app, int r, int g, int b, int a)
{
    if (app->freed)
    {
        return;
    }
    if (SDL_SetRenderDrawColor(app->resources.renderer, r, g, b, a))
    {
        fprintf(stderr, "Failed to set render draw color: %s\n", SDL_GetError());
        return;
    }
    if (SDL_RenderClear(app->resources.renderer))
    {
        fprintf(stderr, "Failed to clear renderer: %s\n", SDL_GetError());
        return;
    }
}

int draw_text(app_t* app, const char* text, int x, int y, int r, int g, int b, int a)
{
    if (app->freed)
    {
        return -1;
    }
    SDL_Color color = {r, g, b, a};
    SDL_Surface* surface = TTF_RenderText_Solid(app->font.ptr, text, color);
    if (!surface)
    {
        fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
        return -1;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->resources.renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Rect rect = {x, y, surface->w, surface->h};
    if (SDL_RenderCopy(app->resources.renderer, texture, NULL, &rect))
    {
        fprintf(stderr, "Failed to render copy: %s\n", SDL_GetError());
        return -1;
    }
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return 0;
}

SDL_Rect* screen_dim(app_t* app, SDL_Rect* rect)
{
    if (app->freed)
    {
        return NULL;
    }
    SDL_GetWindowSize(app->resources.window, &rect->w, &rect->h);
    if (rect)
    {
        rect->x = 0;
        rect->y = 0;
    }
    app->screen.w = rect->w;
    app->screen.h = rect->h;
    return rect;
}

double calculate_upscale_ratio(Uint64 selection_time, Uint64 last_selection_time, int is_selected)
{
    double max_ratio = 1.4;
    double min_ratio = 1.25;
    double ratio = min_ratio;
    if (is_selected)
    {
        if (selection_time > 0)
        {
            if (selection_time < 75)
            {
                ratio = min_ratio + ((double) selection_time / (75.0 / (max_ratio - min_ratio)));
            }
            else
            {
                ratio = max_ratio;
            }
        }
    }
    else
    {
        if (last_selection_time > 0)
        {
            if (last_selection_time < 40)
            {
                ratio = max_ratio - ((double) last_selection_time / (40.0 / (max_ratio - min_ratio)));
            }
            else
            {
                ratio = min_ratio;
            }
        }
    }
    return ratio;
}

void update_pieces_selection_time(app_t* app)
{
    if (app->freed)
    {
        return;
    }
    for (size_t color = 0; color < 2; ++color)
    {
        for (size_t num = 0; num < 16; ++num)
        {
            if (app->pieces[color][num].is_selected)
            {
                app->pieces[color][num].selection_time += app->last_frame_time;
                app->pieces[color][num].last_selection_time = 0;
            }
            else
            {
                app->pieces[color][num].last_selection_time += app->last_frame_time;
                app->pieces[color][num].selection_time = 0;
            }
        }
    }
}

void handle_pieces_selection(app_t* app)
{
    if (app->freed)
    {
        return;
    }
    SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
    for (size_t color = 0; color < 2; ++color)
    {
        for (size_t num = 0; num < 16; ++num)
        {
            if (app->mouse.buttons[0].state == DOWN)
            {
                if (!app->pieces[color][num].is_selected && !app->mouse.is_selecting)
                {
                    if (app->mouse.x >= app->pieces[color][num].rect.x && app->mouse.x <= app->pieces[color][num].rect.x + app->pieces[color][num].rect.w)
                    {
                        if (app->mouse.y >= app->pieces[color][num].rect.y && app->mouse.y <= app->pieces[color][num].rect.y + app->pieces[color][num].rect.h)
                        {
                            app->pieces[color][num].is_selected = 1;
                            app->pieces[color][num].selected_pos.x = (double) app->mouse.x;
                            app->pieces[color][num].selected_pos.y = (double) app->mouse.y;
                            app->mouse.is_selecting = 1;
                        }
                    }
                }
                else if (app->pieces[color][num].is_selected)
                {
                    app->pieces[color][num].selected_pos.x = (double) app->mouse.x;
                    app->pieces[color][num].selected_pos.y = (double) app->mouse.y;
                }
            }
            else
            {
                if (app->pieces[color][num].is_selected)
                {
                    app->mouse.is_selecting = 0;
                    // if it has been dropped on the board, find the closest square and move it there
                    // else, drop it where it was
                    if ((double) app->mouse.x >= app->board.x && (double) app->mouse.x <= app->board.x + app->board.w && (double) app->mouse.y >= app->board.y && (double) app->mouse.y <= app->board.y + app->board.h)
                    {
                        /* double square_h = (app->board.h / 8);
                        double square_w = square_h; */
                        for (size_t i = 0; i < 8; ++i)
                        {
                            for (size_t j = 0; j < 8; ++j)
                            {
                                #if 0
                                if ((double) app->mouse.x >= app->board.x + (i * square_w) && (double) app->mouse.x <= app->board.x + ((i + 1) * square_w))
                                {
                                    if ((double) app->mouse.y >= app->board.y + (j * square_h) && (double) app->mouse.y <= app->board.y + ((j + 1) * square_h))
                                    {
                                        app->pieces[color][num].normal_pos.x = i;
                                        app->pieces[color][num].normal_pos.y = j;
                                        app->pieces[color][num].previous_pos.x = i;
                                        app->pieces[color][num].previous_pos.y = j;
                                        app->pieces[color][num].is_selected = 0;
                                    }
                                }
                                #endif
                                if ((double) app->mouse.x >= app->board.squares[i][j].x && (double) app->mouse.x <= app->board.squares[i][j].x + app->board.squares[i][j].w)
                                {
                                    if ((double) app->mouse.y >= app->board.squares[i][j].y && (double) app->mouse.y <= app->board.squares[i][j].y + app->board.squares[i][j].h)
                                    {
                                        app->pieces[color][num].normal_pos.x = i;
                                        app->pieces[color][num].normal_pos.y = j;
                                        app->pieces[color][num].previous_pos.x = i;
                                        app->pieces[color][num].previous_pos.y = j;
                                        app->pieces[color][num].is_selected = 0;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // Drop it where it was before being selected
                        app->pieces[color][num].is_selected = 0;

                        app->pieces[color][num].normal_pos.x = app->pieces[color][num].previous_pos.x;
                        app->pieces[color][num].normal_pos.y = app->pieces[color][num].previous_pos.y;
                    }
                }
            }

        }
    }
}

int draw_chess_board(app_t* app)
{
    SDL_Rect rect;
    SDL_Rect screen = {0};
    if (screen_dim(app, &screen) == NULL)
    {
        return -1;
    }
    double max_dim = (double) MAX(screen.w, screen.h);
    double min_dim = (double) MIN(screen.w, screen.h);

    double x_offset = 0.0;
    double y_offset = 0.0;

    if (FLOAT_EQ(min_dim, (double) screen.w))
    {
        x_offset = min_dim / 16;
        y_offset = ((max_dim - min_dim) / 2) + (min_dim / 16);
    }
    else
    {
        x_offset = ((max_dim - min_dim) / 2) + (min_dim / 16);
        y_offset = min_dim / 16;
    }

    rect.w = (int) ((min_dim - (min_dim / 8)) / 8);
    rect.h = (int) ((min_dim - (min_dim / 8)) / 8);

    for (size_t i = 0; i < 8; ++i)
    {
        rect.x = (int) (x_offset + (i * rect.w));
        for (size_t j = 0; j < 8; ++j)
        {
            rect.y = (int) (y_offset + (j * rect.h));
            if ((i + j) % 2 == 0)
            {
                if (SDL_SetRenderDrawColor(app->resources.renderer, 238, 238, 210, 255))
                {
                    fprintf(stderr, "Failed to set render draw color: %s\n", SDL_GetError());
                    return -1;
                }
            }
            else
            {
                if (SDL_SetRenderDrawColor(app->resources.renderer, 118, 150, 86, 255))
                {
                    fprintf(stderr, "Failed to set render draw color: %s\n", SDL_GetError());
                    return -1;
                }
            }
            if (SDL_RenderFillRect(app->resources.renderer, &rect))
            {
                fprintf(stderr, "Failed to fill rect: %s\n", SDL_GetError());
                return -1;
            }
            app->board.squares[i][j].x = rect.x;
            app->board.squares[i][j].y = rect.y;
            app->board.squares[i][j].w = rect.w;
            app->board.squares[i][j].h = rect.h;
        }
    }

    app->board.x = x_offset;
    app->board.y = y_offset;
    app->board.w = x_offset + (8 * ((min_dim - (min_dim / 8)) / 8));
    app->board.h = y_offset + (8 * ((min_dim - (min_dim / 8)) / 8));

    if (app->font.size != (size_t) BOARD_FONT)
    {
        if (TTF_SetFontSize(app->font.ptr, BOARD_FONT))
        {
            fprintf(stderr, "Failed to set font size: %s\n", TTF_GetError());
            return -1;
        }
        app->font.size = BOARD_FONT;
    }
    // Draw the letters and numbers
    char letters[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char numbers[8] = {'8', '7', '6', '5', '4', '3', '2', '1'};
    if (app->pov == BLACK_POV)
    {
        REVERSE_ARR(letters, 8);
        REVERSE_ARR(numbers, 8);
    }
    for (size_t i = 0; i < 8; ++i)
    {
        char let[2] = {letters[i], '\0'};
        char num[2] = {numbers[i], '\0'};
        int x_let, x_num, y_let, y_num;
        x_let = (int) (x_offset + (((double) i + 0.4) * rect.w));
        y_let = (int) (y_offset - (min_dim / 16));
        x_num = (int) (x_offset - (min_dim / 16)) + (0.1 * rect.w);
        y_num = (int) (y_offset + (((double) i + 0.3) * rect.h));
        if (draw_text(app, let, x_let, y_let, 255, 255, 255, 255))
        {
            return -1;
        }
        if (draw_text(app, num, x_num, y_num, 255, 255, 255, 255))
        {
            return -1;
        }
    }

    // Draw the pieces
    for (size_t i = 0; i < 16; ++i)
    {
        if (app->pieces[WHITE][i].is_dead)
        {
            continue;
        }
        double ratio = calculate_upscale_ratio(app->pieces[WHITE][i].selection_time, app->pieces[WHITE][i].last_selection_time, app->pieces[WHITE][i].is_selected);
        SDL_Rect piece_rect = {0};
        piece_rect.w = (int) ((double) rect.w * 0.7 * ratio);
        piece_rect.h = (int) ((double) rect.h * 0.7 * ratio);
        if (!app->pieces[WHITE][i].is_selected)
        {
            piece_rect.x = (int) (x_offset + (app->pieces[WHITE][i].normal_pos.x * (double) rect.w)) 
                         + (((double) rect.w - (double) piece_rect.w) / 2);
            piece_rect.y = (int) (y_offset + (app->pieces[WHITE][i].normal_pos.y * (double) rect.h)) 
                         + (((double) rect.h - (double) piece_rect.h) / 2);
        }
        else
        {
            piece_rect.x = (int) (app->pieces[WHITE][i].selected_pos.x - ((double) piece_rect.w / 2));
            piece_rect.y = (int) (app->pieces[WHITE][i].selected_pos.y - ((double) piece_rect.h / 2));
        }
        #if 0
        SDL_Texture* piece_texture = NULL;
        if (i <= 7)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[PAWN].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 9)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[KNIGHT].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 11)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[BISHOP].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 13)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[ROOK].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 14)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[QUEEN].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces_surfaces[KING].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        #endif
        if (SDL_RenderCopy(app->resources.renderer, app->white_pieces_surfaces[i <= 7 ? PAWN : i <= 9 ? KNIGHT : i <= 11 ? BISHOP : i <= 13 ? ROOK : i <= 14 ? QUEEN : KING].texture, NULL, &piece_rect))
        {
            fprintf(stderr, "Failed to render copy: %s\n", SDL_GetError());
            return -1;
        }
        app->pieces[WHITE][i].rect = piece_rect;
    }
    for (size_t i = 0; i < 16; ++i)
    {
        if (app->pieces[BLACK][i].is_dead)
        {
            continue;
        }
        double ratio = calculate_upscale_ratio(app->pieces[BLACK][i].selection_time, app->pieces[BLACK][i].last_selection_time, app->pieces[BLACK][i].is_selected);
        SDL_Rect piece_rect = {0};
        piece_rect.w = (int) ((double) rect.w * 0.7 * ratio);
        piece_rect.h = (int) ((double) rect.h * 0.7 * ratio);
        if (!app->pieces[BLACK][i].is_selected)
        {
            piece_rect.x = (int) (x_offset + (app->pieces[BLACK][i].normal_pos.x * (double) rect.w)) 
                         + (((double) rect.w - (double) piece_rect.w) / 2);
            piece_rect.y = (int) (y_offset + (app->pieces[BLACK][i].normal_pos.y * (double) rect.h)) 
                         + (((double) rect.h - (double) piece_rect.h) / 2);
        }
        else
        {
            piece_rect.x = (int) (app->pieces[BLACK][i].selected_pos.x - ((double) piece_rect.w / 2));
            piece_rect.y = (int) (app->pieces[BLACK][i].selected_pos.y - ((double) piece_rect.h / 2));
        }
        #if 0
        SDL_Texture* piece_texture = NULL;
        if (i <= 7)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[PAWN].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 9)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[KNIGHT].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 11)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[BISHOP].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 13)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[ROOK].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else if (i <= 14)
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[QUEEN].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        else
        {
            piece_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->black_pieces_surfaces[KING].surface);
            if (!piece_texture)
            {
                fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
                return -1;
            }
        }
        #endif
        if (SDL_RenderCopy(app->resources.renderer, app->black_pieces_surfaces[i <= 7 ? PAWN : i <= 9 ? KNIGHT : i <= 11 ? BISHOP : i <= 13 ? ROOK : i <= 14 ? QUEEN : KING].texture, NULL, &piece_rect))
        {
            fprintf(stderr, "Failed to render copy: %s\n", SDL_GetError());
            return -1;
        }
        app->pieces[BLACK][i].rect = piece_rect;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    app_t* app = init_game(NULL);
    int running = 1;
    if (!app)
    {
        return -1;
    }
    while (running)
    {
        Uint64 frame_start = SDL_GetTicks64();
        Uint64 frame_end = 0;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
                break;
            }
            else
            {
                switch (event.type)
                {
                    case SDL_MOUSEMOTION:
                    {
                        app->mouse.x = event.motion.x;
                        app->mouse.y = event.motion.y;
                        break;
                    }
                    case SDL_MOUSEBUTTONDOWN:
                    {
                        switch (event.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                            {
                                app->mouse.buttons[0].state = DOWN;
                                break;
                            }
                            case SDL_BUTTON_MIDDLE:
                            {
                                app->mouse.buttons[1].state = DOWN;
                                break;
                            }
                            case SDL_BUTTON_RIGHT:
                            {
                                app->mouse.buttons[2].state = DOWN;
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONUP:
                    {
                        switch (event.button.button)
                        {
                            case SDL_BUTTON_LEFT:
                            {
                                app->mouse.buttons[0].state = UP;
                                break;
                            }
                            case SDL_BUTTON_MIDDLE:
                            {
                                app->mouse.buttons[1].state = UP;
                                break;
                            }
                            case SDL_BUTTON_RIGHT:
                            {
                                app->mouse.buttons[2].state = UP;
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            #if defined(_WIN32)
            Sleep((DWORD) (1));
            #elif defined(__linux__)
            usleep((useconds_t) (1000));
            #endif
        }
        frame_end = SDL_GetTicks64();
        double elapsed_ms = (double) (frame_end - frame_start);
        if (elapsed_ms < EXPECTED_FRAME_TIME)
        {
            #if defined(_WIN32)
            Sleep((DWORD) (EXPECTED_FRAME_TIME - elapsed_ms));
            #elif defined(__linux__)
            usleep((useconds_t) ((EXPECTED_FRAME_TIME - elapsed_ms) * 1000));
            #endif
        }
        #if 0
        else
        {
            frame_end = SDL_GetTicks64();
            double elapsed_ms = (double) (frame_end - frame_start);
            if (elapsed_ms < EXPECTED_FRAME_TIME)
            {
                #if defined(_WIN32)
                Sleep((DWORD) (EXPECTED_FRAME_TIME - elapsed_ms));
                #elif defined(__linux__)
                usleep((useconds_t) ((EXPECTED_FRAME_TIME - elapsed_ms) * 1000));
                #endif
            }
        }
        #endif
        if (running)
        {
            app->last_frame_time = SDL_GetTicks64() - frame_start;
            handle_pieces_selection(app);
            update_pieces_selection_time(app);
            clear_screen(app, BOARD_BACKGROUND);
            if (draw_chess_board(app))
            {
                running = 0;
                break;
            }
            SDL_RenderPresent(app->resources.renderer);
        }
    }
    
    quit_game(app);

    free(app);
    return 0;
}
