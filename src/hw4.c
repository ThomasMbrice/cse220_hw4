#include "hw4.h"

void initialize_game(ChessGame *game) {
    char arr_nobles[] = {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'};

    for(int i = 0; i < 8; i++){
        for(int e= 0; e< 8; e++){
            if(i== 0)
                game->chessboard[i][e] = arr_nobles[e];    // create black's nobles
            else if(i == 7)
                game->chessboard[i][e] = toupper(arr_nobles[e]);    // create white's nobles
            else if(i == 1)
                game->chessboard[i][e] = 'p';               // create blac's pawns 
            else if(i == 6)
                game -> chessboard[i][e] = 'P';             // create whites's pawns
            else
                game -> chessboard[i][e] = '.';             // create emptey space 
        }
    }
    game->capturedCount = 0;
    game->moveCount = 0;
    game->currentPlayer = 0;
}

void chessboard_to_fen(char fen[], ChessGame *game) {
    int spacecounter = 0, count = 0;
    for(int i = 0; i < 8; i++){
        for(int e= 0; e < 8;e++){
            if(game->chessboard[i][e] == '.'){          // if space needs to be counted
                spacecounter++;
                continue;
            }
            else{
            if(spacecounter > 0){                   //if space has been counted
                fen[count++] = spacecounter + '0';
                spacecounter = 0;   
            }
            fen[count++] =game->chessboard[i][e];       // if space NA
            }
        }
        
        if(spacecounter > 0)                            // emptey space at end
        fen[count++] = spacecounter + '0';
        if(i != 7)
        fen[count++] = '/';                             // dash

        spacecounter = 0;                               // rest coutne    
    }

    fen[count++] = ' ';                             // space for some reason
    if(game->currentPlayer == 0)
        fen[count++] = 'w';                         // whos turn it is
    else   
        fen[count++] = 'b';
    
    fen[count++] = '\0';                            // null pointer
}

// int side_to_side(int row, int col, ChessGame *game, char direction){ // 1 if left 2 if right 0 if non
//     if((row < 0 || row > 8) || (col < 0 || col > 8) || game == NULL){
//         printf("INVALID INNIT COND KILLER %d %d\n",row, col);      //invalid innit cond
//         return false;
//     }
//     if(direction == 'H'){
//         if(col-1 > 0 && game->chessboard[row][col-1] != '.')
//             return 1;
//         else if(col+1 < 8 && game->chessboard[row][col+1] != '.')
//             return 2;
//         else
//             return 0;
//     }
//     else if(direction == 'V'){
//         if(row-1 > 0 && game->chessboard[row-1][col] != '.')
//             return 1;
//         else if(row+1 < 8 && game->chessboard[row+1][col] != '.')
//             return 2;
//         else
//             return 0;
//     }
//     else
//     return 0;
// }

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if((src_row < 0 || src_row > 8) || (src_col < 0 || src_col > 8) 
    || (dest_row < 0 || dest_row > 8) || (dest_col < 0 || dest_col > 8) || game == NULL){
        printf("INVALID INNIT COND PAWNMOVE%d %d %d %d\n",src_row, src_col, dest_row, dest_col);      //invalid innit cond
        return false;
    }

    if(piece == 'p'){   // black moves torwards 0
        if(src_row == 1 && dest_row - src_row ==2 && src_col == dest_col
        && game->chessboard[src_row+1][src_col] == '.' && game->chessboard[dest_row][dest_col] == '.') 
            return true;        //start allows move of 2
        else if(dest_row - src_row == 1 && src_col == dest_col && game->chessboard[dest_row][dest_col] == '.'){
            return true;         // down one
        }
        else if(src_col != dest_col && dest_row - src_row == 1 && game->chessboard[dest_row][dest_col] != '.'){ //kill 
            return true; //kill 
        }
        else
            return false;
    }
    else if(piece == 'P'){  // white moves torwards 7
        if(src_row == 6 && src_row - dest_row ==2 && src_col == dest_col 
        && game->chessboard[src_row-1][dest_col] == '.' && game->chessboard[dest_row][dest_col] == '.')   
            return true;    //start allows move of 2
        else if(src_row - dest_row ==1 && src_col == dest_col && game->chessboard[dest_row][dest_col] == '.'){ 
            return true;    // move up one
        }
        else if(src_col != dest_col && src_row - dest_row ==1 && game->chessboard[dest_row][dest_col] != '.'){ 
            return true;    //kill
        }
        else
            return false;
    }
    else
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if((src_row < 0 || src_row > 8) || (src_col < 0 || src_col > 8) 
    || (dest_row < 0 || dest_row > 8) || (dest_col < 0 || dest_col > 8) || game == NULL){
        printf("INVALID INNIT COND CASTLE%d %d %d %d\n",src_row, src_col, dest_row, dest_col);      //invalid innit cond
        return false;
    }
    int start = 0, end = 0;

    if(game->chessboard[dest_row][dest_col] != '.' && 
    isupper(game->chessboard[src_row][src_col])==isupper(game->chessboard[dest_row][dest_col])){
        //printf("dest and src are same\n");
        return false;
    }

    if(src_row != dest_row && src_col == dest_col){ // vertical 
        if(src_row > dest_row){
            start = dest_row;
            end = src_row;
        }
        else{
            end = dest_row;
            start = src_row;
        }

        for(int i = start+1; i < end-1; i++){ //checks if road is clear
            if(game->chessboard[i][src_col] != '.')
                return false;
        }
                return true;

    }
    else if(src_col != dest_col && src_row == dest_row){ // horizontal 
        if(src_col > dest_col){
            start = dest_col;
            end = src_col;
        }
        else{
            end = dest_col;
            start = src_col;
        }

        if(game->chessboard[src_row][start+1] != '.')
            return false;

        for(int i = start+1; i < end-1; i++){ //checks if road is clear
            if(game->chessboard[src_row][i] != '.')
                return false;
        }
        return true;

    }
    else
        return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    if((src_row < 0 || src_row > 8) || (src_col < 0 || src_col > 8) 
    || (dest_row < 0 || dest_row > 8) || (dest_col < 0 || dest_col > 8)){
        printf("INVALID INNIT COND HORSE%d %d %d %d\n",src_row, src_col, dest_row, dest_col);      //invalid innit cond
        return false;
    }
    int difrow = abs(src_row-dest_row), difcol = abs(src_col-dest_col);

    if((difrow == 1 && difcol == 2) || (difrow == 2 && difcol == 1)){
        return true;
    }
    else
        return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if((src_row < 0 || src_row > 8) || (src_col < 0 || src_col > 8) 
    || (dest_row < 0 || dest_row > 8) || (dest_col < 0 || dest_col > 8) || game == NULL){
        printf("INVALID INNIT COND bishop%d %d %d %d\n",src_row, src_col, dest_row, dest_col);      //invalid innit cond
        return false;
    }
    
    int quadrant = 0; //SPLIT BOARD INTO 4 QUADRANTS BASED ON WHERE THE PEICE IS
    
    if(game->chessboard[dest_row][dest_col] != '.' && 
    isupper(game->chessboard[src_row][src_col]) == isupper(game->chessboard[dest_row][dest_col])){
        //printf("dest and src are same\n");
        return false;
    }

    if((abs(src_col - dest_col) / abs(src_row - dest_row)) != 1){
        printf("bishop col/row is not 1\n");
        return false;
    }
    

    if(dest_row > src_row && dest_col > src_col)
        quadrant = 1;                   // destination to to the top right
    else if(dest_row > src_row && dest_col < src_col)
        quadrant = 2;                   //destination is to top left
    else if(dest_row < src_row && dest_col < src_col)
        quadrant = 3;                   // destination is to the bottom left
    else if(dest_row < src_row && dest_col > src_col)
        quadrant = 4;                   // destination is to the bottom right

    if(quadrant == 0)
        return false;


    int i = src_row, e = src_col, increments = 0, max = abs(src_col - dest_col)-1;
    while(increments < max){
            if(quadrant == 1){
                i++;
                e++;
            }
            if(quadrant == 2){
                i++;
                e--;
            }
            if(quadrant == 3){
                i--;
                e--;
            }
            if(quadrant == 4){
                i--;
                e++;
            }
            if(game->chessboard[i][e] != '.')
                return false;
            increments++;
        }

    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(src_row == dest_row || src_col == dest_col)
        return is_valid_rook_move(src_row, src_col,dest_row,dest_col,game);
    else
        return is_valid_bishop_move(src_row, src_col,dest_row,dest_col,game);
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    if((src_row < 0 || src_row > 8) || (src_col < 0 || src_col > 8) 
    || (dest_row < 0 || dest_row > 8) || (dest_col < 0 || dest_col > 8)){
        printf("invalid innit con KING%d %d %d %d\n",src_row, src_col, dest_row, dest_col);      //invalid innit cond
    return false;
    }

    if((abs(src_row-dest_row) <= 1 && abs(src_col - dest_col) <= 1))
        return true;

    return false;


}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if(toupper(piece) == 'P')   //pawn
        return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
    else if(toupper(piece) == 'R')  // rook
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
    else if(toupper(piece) == 'B')  // bishop
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
    else if(toupper(piece) == 'N') // knight
        return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
    else if(toupper(piece) == 'Q')  // queen
        return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
    else if(toupper(piece) == 'K')  // king
        return is_valid_king_move(src_row, src_col, dest_row, dest_col);
    else
        return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    int count = 0, x = 0, y = 0, num = 0;
    char set[8][9];
    while(!isspace(fen[count]) && x < 8){
        while(fen[count] != '/' && y < 8){
            set[x][y++] = fen[count++];
        }
    count++;
    set[x][y] = '\0';
    x++;
    y = 0;
    }   
    y = 0;
    int temp = 0;

    for(int i = 0; i < 8; i++){
        while(set[i][y] != '\0' && y < 8 && num < 8){
            if (set[i][y] < '1' || set[i][y] > '8') {           //if not number
                game->chessboard[i][num] = set[i][y];
                num++;
            }
            else{
                temp = set[i][y] - '0';
                while(temp > 0 && num < 8){
                    game->chessboard[i][num] = '.';
                    num++;
                    temp--;
                }
            }
            y++;        //col increases every rotation of the loop
        }
    num = 0;
    y= 0;
    }

    if(fen[strlen(fen)-1] == 'b'){
       // printf("char is %c ", fen[strlen(fen)-1]);
        game->currentPlayer = 1;
    }
    else if(fen[strlen(fen)-1] == 'w'){
       // printf("char is %c ", fen[strlen(fen)-1]);
        game->currentPlayer = 0;
    }
    else
        printf("ERROR: Invalid current player marker\n");
}
// while(!isspace(fen[count]) || strlen(fen) > (size_t)count){ 
        
//         if(x == 8)
//             break;              // end if at last row

//         while(fen[count] != '/'){
//             if(fen[count] >= '1' && fen[count] <= '8'){
//                 num = fen[count] - '0';
//                 for(int i = 0; i < num; i++){
//                     game->chessboard[x][y++] = '.';
//                 }
//                 count++;
//             }
//             else{
//                 game->chessboard[x][y] = fen[count++];
//                 y++;
//                 count++;
//             }
//         }
//         count++;                // move past '/'
//         x++;                    // rest increment x
//         y = 0;                  // reset y
        
//     }
//      count++; // Move past the whitespace
//      printf("\ncurrent player %c count %d \n", fen[count], count);
//      printf("\ntotal %s\n", fen);


int parse_move(const char *move, ChessMove *parsed_move) {
    if(strlen(move) > 5)                            // add less than 4
        return PARSE_MOVE_INVALID_FORMAT;

    for(int i = 0; i < 4; i++){
        if(i == 2 || i == 0){
            if(move[i] < 'a' || move[i] > 'h'){
                return PARSE_MOVE_INVALID_FORMAT;
            }
        }
        if(i == 1 || i == 3){
            if(move[i] < '1' || move[i] > '8'){
                return PARSE_MOVE_OUT_OF_BOUNDS;
            }
        }
    }
    if((strlen(move) == 4 || strlen(move) == 5) && (move[0] >= 'a' && move[0] <= 'h')){              //regular move   
        parsed_move->startSquare[0] = move[0];
        parsed_move->startSquare[1] = move[1];
        parsed_move->startSquare[2] = '\0';
        parsed_move->endSquare[0] = move[2];
        parsed_move->endSquare[1] = move[3];
        if(strlen(move) == 5){                   // for pawn promotion
            if(!(move[3] == '8' || move[3] == '1')){
                return PARSE_MOVE_INVALID_DESTINATION;
            }
            if(!(move[4] == 'q' || move[4] == 'r' || move[4] == 'b' || move[4] == 'n'))
                return PARSE_MOVE_INVALID_PROMOTION;

            parsed_move->endSquare[2] = move[4];
            parsed_move->endSquare[3] = '\0';
        } 
        else{
            parsed_move->endSquare[2] = '\0';
        }
    }
    else
        return PARSE_MOVE_INVALID_FORMAT;

    return 0;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    int y = move->startSquare[0]-97, x = move->startSquare[1] - '1' , 
    endy = move->endSquare[0]-97, endx = move->endSquare[1] - '1', 
    movesize = strlen(move->endSquare) + strlen(move->startSquare);             // movesize
    //need to convert from chess to array cords
    x = (abs(8-x-1));
    //y = (abs(8-y-1));
    endx = (abs(8-endx-1));
    //endy = (abs(8-endy-1));

    char peice = game->chessboard[x][y];
    char endpeice = game->chessboard[endx][endy];
    validate_move = is_valid_move(peice, x, y, endx, endy, game);


    if((game->currentPlayer == 0 && is_client == false) || (game->currentPlayer == 1 && is_client == true))    // white tries to move black peice
        return MOVE_OUT_OF_TURN;
    else if(peice == '.')                               // tries to move nothing
        return MOVE_NOTHING;
    else if(is_client == true && tolower(peice) == peice)    // white tries to move black peice
        return MOVE_WRONG_COLOR;
    else if(is_client == false && toupper(peice) == peice)       // black tries to move white
        return MOVE_WRONG_COLOR;
    else if (endpeice != '.' && (isupper(peice) == isupper(endpeice)))
        return MOVE_SUS;                                        // capture own peice
    else if((movesize == 5) && (toupper(peice) != 'P'))
        return MOVE_NOT_A_PAWN;
    else if((movesize == 4) && (toupper(peice) == 'P') && (endy == 0 || endy == 7))
        return MOVE_MISSING_PROMOTION;
    else if(validate_move == false)
        return MOVE_WRONG;
    
    char newpeice = '.';
    game->chessboard[x][y] = '.';           // void current position
    
    if(movesize == 5){                       //checks for promotion
        newpeice = move->endSquare[2];
        if(isupper(peice))                      // smells like promotion weeeeh
            newpeice = toupper(newpeice);
    }

    if(newpeice != '.')
        game->chessboard[endx][endy] = newpeice;    //promoted peice added to board
    else
        game->chessboard[endx][endy] = peice;       // place peice onto board

    if(endpeice != '.'){                             // peive was infact captured
        game->capturedPieces[game->capturedCount++] = endpeice; //upadte that array
    }

    game->moves[game->moveCount++] = *move;         //ince rement

    if(game->currentPlayer == 0)                //change game player
        game->currentPlayer = 1;
    else    
        game->currentPlayer = 0;
    

    return 0;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    char buffer[255];
    int counter = 0;
    while(!isspace(message[counter])){
        buffer[counter] = message[counter];
        counter++;
    }
    buffer[counter] = '\0';
    counter++;
    
    if(strcmp(buffer, "/move") == 0){
        int temp = 0;
        ChessMove c1;
        char move[255];
        while(counter < (int)strlen(message)){
            move[temp] = message[counter];
            counter++;
            temp++;
        }
        move[temp] = '\0';
        if(parse_move(move, &c1) != 0)
            return COMMAND_ERROR;
        if(make_move(game, &c1, is_client, false))            //should this be false?
            return COMMAND_ERROR;
                                                                //what after?
        return COMMAND_MOVE;
    }
    else if(strcmp(buffer, "/forfeit") == 0){
        
        return COMMAND_FORFEIT;
    }
    else if(strcmp(buffer, "/chessboard") == 0){
        display_chessboard(game);
        return COMMAND_DISPLAY;
    }
    else if(strcmp(buffer, "/import") == 0){
        
        return COMMAND_IMPORT;
    }
    else if(strcmp(buffer, "/load") == 0){

    }
    else{
        printf("MAJOR READING COMMAND ERROR \n");
        return COMMAND_UNKNOWN;
    }

    return COMMAND_UNKNOWN;
    // (void)game;
    // (void)message;
    (void)socketfd;
    // (void)is_client;
    // return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    FILE *file = fopen(db_filename, "a");
    if (file == NULL) {
        printf("Error opening file\n");
        return -1;
    }
    if (strchr(username, ' ') != NULL) {
        return -1; // Username contains a space
    }

    // Check if the username is empty
    if (strlen(username) == 0) {
        return -1; // Username is empty
    }

    char fen[1048];
    chessboard_to_fen(fen, game);

    fprintf(file, "%s:%s\n", username, fen);
    fclose(file);

    return 0; // Successful save operation



    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    FILE *file = fopen(db_filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return -1;
    }
    if(save_number < 1)
        return -1;
    
    char line[1048];
    int count = 0, thecount = 0;
    char name[255], fen[255];

    while (fgets(line, sizeof(line), file)) {
    count = 0;

    while (line[count] != ':' && line[count] != '\0') {
        name[count] = line[count];
        count++;
    }
    name[count] = '\0'; 

    if (line[count] == ':') {
        count++;
    }

    int fen_count = 0;
    while (line[count] != '\n' && line[count] != '\0') {
        fen[fen_count] = line[count];
        fen_count++;
        count++;
    }
    fen[fen_count] = '\0'; 

    if (strcmp(name, username) == 0) {
        thecount++;
        if (thecount == save_number) {
            fen_to_chessboard(fen, game);
            fclose(file);
            return 0; // Success
            }
        }
    }
   //             printf("PRINTING LINE %s : %s \n %s \n",name, fen, line);

    fclose(file);
    return -1;          //fail
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
