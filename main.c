#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <stdio.h>

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
#define PIECES_IMG_PATH "./JohnPablok Cburnett Chess set/SVG with shadow/"

#if defined(B_PAWN_IMG) || defined(B_KNIGHT_IMG) || defined(B_BISHOP_IMG) || defined(B_ROOK_IMG) || defined(B_QUEEN_IMG) || defined(B_KING_IMG)
    #undef B_PAWN_IMG
    #undef B_KNIGHT_IMG
    #undef B_BISHOP_IMG
    #undef B_ROOK_IMG
    #undef B_QUEEN_IMG
    #undef B_KING_IMG
#endif
#define B_PAWN_IMG PIECES_IMG_PATH "b_pawn_svg_withShadow.svg"
#define B_KNIGHT_IMG PIECES_IMG_PATH "b_knight_svg_withShadow.svg"
#define B_BISHOP_IMG PIECES_IMG_PATH "b_bishop_svg_withShadow.svg" 
#define B_ROOK_IMG PIECES_IMG_PATH "b_rook_svg_withShadow.svg"
#define B_QUEEN_IMG PIECES_IMG_PATH "b_queen_svg_withShadow.svg"
#define B_KING_IMG PIECES_IMG_PATH "b_king_svg_withShadow.svg"

#if defined(W_PAWN_IMG) || defined(W_KNIGHT_IMG) || defined(W_BISHOP_IMG) || defined(W_ROOK_IMG) || defined(W_QUEEN_IMG) || defined(W_KING_IMG)
    #undef W_PAWN_IMG
    #undef W_KNIGHT_IMG
    #undef W_BISHOP_IMG
    #undef W_ROOK_IMG
    #undef W_QUEEN_IMG
    #undef W_KING_IMG
#endif
#define W_PAWN_IMG PIECES_IMG_PATH "w_pawn_svg_withShadow.svg"
#define W_KNIGHT_IMG PIECES_IMG_PATH "w_knight_svg_withShadow.svg"
#define W_BISHOP_IMG PIECES_IMG_PATH "w_bishop_svg_withShadow.svg"
#define W_ROOK_IMG PIECES_IMG_PATH "w_rook_svg_withShadow.svg"
#define W_QUEEN_IMG PIECES_IMG_PATH "w_queen_svg_withShadow.svg"
#define W_KING_IMG PIECES_IMG_PATH "w_king_svg_withShadow.svg"

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
        struct
        {
            int x;
            int y;
            int is_dead;
        } pos;
    } white_pieces[6];

    struct
    {
        char source_path[512];
        SDL_Surface* surface;
        struct
        {
            int x;
            int y;
            int is_dead;
        } pos;
    } black_pieces[6];

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
            NONE_SELECTED,
            PAWN_SELECTED,
            KNIGHT_SELECTED,
            BISHOP_SELECTED,
            ROOK_SELECTED,
            QUEEN_SELECTED,
            KING_SELECTED
        } selected_piece;

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
    } board;

    struct 
    {
        SDL_Window *window;
        SDL_Renderer *renderer;
    } resources;
    
    struct 
    {
        int x;
        int y;
    } mouse;
    
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

    int freed;

} app_t;

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
        return NULL;
    }
    app->resources.window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MOUSE_CAPTURE | SDL_WINDOW_MAXIMIZED);
    if (!app->resources.window)
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return NULL;
    }
    app->resources.renderer = SDL_CreateRenderer(app->resources.window, -1, SDL_RENDERER_ACCELERATED);
    if (!app->resources.renderer)
    {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        return NULL;
    }
    
    if (SDL_SetRenderDrawColor(app->resources.renderer, BOARD_BACKGROUND))
    {
        fprintf(stderr, "Failed to set render draw color: %s\n", SDL_GetError());
        return NULL;
    }
    if (SDL_RenderClear(app->resources.renderer))
    {
        fprintf(stderr, "Failed to clear renderer: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_RenderPresent(app->resources.renderer);
    app->freed = 0;
    if (SDL_CaptureMouse(SDL_TRUE))
    {
        fprintf(stderr, "Failed to capture mouse: %s\n", SDL_GetError());
    }
    SDL_GetMouseState(&app->mouse.x, &app->mouse.y);
    SDL_GetWindowSize(app->resources.window, &app->screen.w, &app->screen.h);
    app->board.x = 0.0;
    app->board.y = 0.0;
    app->board.w = 0.0;
    app->board.h = 0.0;
    if (TTF_Init())
    {
        fprintf(stderr, "Failed to init TTF: %s\n", TTF_GetError());
        return NULL;
    }
    app->font.ptr = TTF_OpenFont(HACK_FONT_PATH, BOARD_FONT);
    if (!app->font.ptr)
    {
        fprintf(stderr, "Failed to open font: %s\n", TTF_GetError());
        return NULL;
    }
    app->font.size = BOARD_FONT;
    app->pov = WHITE_POV;
    for (size_t i = 0; i < 6; ++i)
    {
        memset(app->white_pieces[i].source_path, 0, 512);
        memset(app->black_pieces[i].source_path, 0, 512);
        app->black_pieces[i].pos.is_dead = 0;
        app->white_pieces[i].pos.is_dead = 0;
    }
    strcpy(app->white_pieces[0].source_path, W_PAWN_IMG);
    strcpy(app->white_pieces[1].source_path, W_KNIGHT_IMG);
    strcpy(app->white_pieces[2].source_path, W_BISHOP_IMG);
    strcpy(app->white_pieces[3].source_path, W_ROOK_IMG);
    strcpy(app->white_pieces[4].source_path, W_QUEEN_IMG);
    strcpy(app->white_pieces[5].source_path, W_KING_IMG);
    strcpy(app->black_pieces[0].source_path, B_PAWN_IMG);
    strcpy(app->black_pieces[1].source_path, B_KNIGHT_IMG);
    strcpy(app->black_pieces[2].source_path, B_BISHOP_IMG);
    strcpy(app->black_pieces[3].source_path, B_ROOK_IMG);
    strcpy(app->black_pieces[4].source_path, B_QUEEN_IMG);
    strcpy(app->black_pieces[5].source_path, B_KING_IMG);
    for (size_t i = 0; i < 6; ++i)
    {
        app->white_pieces[i].surface = NULL;
        app->black_pieces[i].surface = NULL;
    }
    for (size_t i = 0; i < 6; ++i)
    {
        app->white_pieces[i].surface = IMG_Load(app->white_pieces[i].source_path);
        if (!app->white_pieces[i].surface)
        {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            return NULL;
        }
        app->black_pieces[i].surface = IMG_Load(app->black_pieces[i].source_path);
        if (!app->black_pieces[i].surface)
        {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            return NULL;
        }
    }

    app->states.turn = WHITE;
    app->states.app_state = MENU;
    app->states.selected_piece = NONE_SELECTED;
    app->states.game_state = GAME_NOT_STARTED;

    // TODO: Pieces positions
    
    return app;
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

void quit_game(app_t* app)
{
    SDL_DestroyRenderer(app->resources.renderer);
    SDL_DestroyWindow(app->resources.window);
    for (size_t i = 0; i < 6; ++i)
    {
        SDL_FreeSurface(app->white_pieces[i].surface);
        SDL_FreeSurface(app->black_pieces[i].surface);
    }
    TTF_CloseFont(app->font.ptr);
    TTF_Quit();
    SDL_Quit();
    app->freed = 1;
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
                if (SDL_SetRenderDrawColor(app->resources.renderer, 242, 214, 211, 255))
                {
                    fprintf(stderr, "Failed to set render draw color: %s\n", SDL_GetError());
                    return -1;
                }
            }
            else
            {
                if (SDL_SetRenderDrawColor(app->resources.renderer, 41, 72, 120, 255))
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
        }
    }

    app->board.x = x_offset;
    app->board.y = y_offset;
    app->board.w = x_offset + (8 * rect.w);
    app->board.h = y_offset + (8 * rect.h);

    if (app->font.size != BOARD_FONT)
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

    // Test: try to render a queen
    SDL_Rect queen_rect = {0};
    queen_rect.w = (int) ((double) rect.w * 0.8);
    queen_rect.h = (int) ((double) rect.h * 0.8);
    queen_rect.x = (int) (x_offset + (3 * (double) rect.w) + ((double) rect.w * 0.1));
    queen_rect.y = (int) (y_offset + (3 * (double) rect.h) + ((double) rect.h * 0.1));
    SDL_Texture* queen_texture = SDL_CreateTextureFromSurface(app->resources.renderer, app->white_pieces[QUEEN].surface);
    if (!queen_texture)
    {
        fprintf(stderr, "Failed to create texture from surface: %s\n", SDL_GetError());
        return -1;
    }
    if (SDL_RenderCopy(app->resources.renderer, queen_texture, NULL, &queen_rect))
    {
        fprintf(stderr, "Failed to render copy: %s\n", SDL_GetError());
        return -1;
    }
    SDL_DestroyTexture(queen_texture);
    
    return 0;
}

int main(int argc, char *argv[])
{
    app_t* app = init_game(NULL);
    if (!app)
    {
        return -1;
    }
    while (1)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
            clear_screen(app, BOARD_BACKGROUND);
            if (draw_chess_board(app))
            {
                break;
            }
            SDL_RenderPresent(app->resources.renderer);
        }
        else
        {
            SDL_Delay((Uint32) (1000.0 / 140.0));
        }
    }
    
    quit_game(app);

    free(app);
    return 0;
}