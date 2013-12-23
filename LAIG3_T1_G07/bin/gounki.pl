/* -*- Mode:Prolog; coding:iso-8859-1; -*- */
:-dynamic(player/3).
:-use_module(library(system)).
:-use_module(library(random)).
:-use_module(library(sockets)).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



port(60070).

% launch me in sockets mode
server:-
        port(Port),
        socket_server_open(Port, Socket),
        socket_server_accept(Socket, _Client, Stream, [type(text)]),
        write('Accepted connection'), nl,
        serverLoop(Stream),
        socket_server_close(Socket).

% wait for commands
serverLoop(Stream) :-
        repeat,
        read(Stream, ClientMsg),
        write('Received: '), write(ClientMsg), nl,
        parse_input(ClientMsg, MyReply),
        format(Stream, '~q.~n', [MyReply]),
        write('Wrote: '), write(MyReply), nl,
        flush_output(Stream),
        (ClientMsg == quit; ClientMsg == end_of_file), !.

parse_input(check_end_of_game(Arg1), Answer) :-
        check_end_of_game(Arg1, Answer).

parse_input(comando(Arg1, Arg2), Answer) :-
        comando(Arg1, Arg2, Answer).
        
parse_input(play(Arg1,Arg2,Arg3,Arg4,Arg5), Answer) :-
        play(Arg1, Arg2, Arg3, Arg4,Arg5, Answer).

parse_input(pc_move(Arg1,Arg2,Arg3), Answer) :-
        pc_move(Arg1, Arg2, Arg3, Answer),!.

parse_input(quit, ok-bye) :- !.
                
comando(Arg1, Arg2, Answer) :-
        write(Arg1), nl, write(Arg2), nl,
        Answer = 5.


%%%%%%%%%%%%%%%% Initial chooses %%%%%%%%%%%%%%%%
                                
choose_mode(A) :-
        retractall(player(_,_,_)),
        write('This game can be played in 4 modes:'),nl,
        write(' 1. Human vs Human'),nl,
        write(' 2. Human vs PC'),nl,
        write(' 3. PC vs Human'),nl,
        write(' 4. PC vs PC'),nl,
        read(A),
        integer(A),
        choose_mode_aux(A).
        
choose_mode(A) :- 
        retractall(player(_,_,_)),
        choose_mode(A).

choose_mode_aux(1) :-
        assert(player(1,human,0)),
        assert(player(2,human,0)).

choose_mode_aux(2) :-
        assert(player(1,human,0)),
        write('Which difficulty do you want for pc? (1-3)'),nl,
        read(D),
        integer(D),
        D >= 1,
        D =< 3,
        assert(player(2,pc,D)).

choose_mode_aux(3) :-
        assert(player(2,human,0)),
        write('Which difficulty do you want for pc? (1-3)'),nl,
        read(D),
        integer(D),
        D >= 1,
        D =< 3,
        assert(player(1,pc,D)).

choose_mode_aux(4) :-
        write('Which difficulty do you want for pc 1? (1-3)'),nl,
        read(D),
        integer(D),
        D >= 1,
        D =< 3,
        write('Which difficulty do you want for pc 2? (1-3)'),nl,
        read(D1),
        integer(D1),
        D1 >= 1,
        D1 =< 3,
        assert(player(1,pc,D)),
        assert(player(2,pc,D1)).

                
%%%%%%%%%%%%%%%% Printing board %%%%%%%%%%%%%%%%

print_symbol(x) :- write('X').
print_symbol(o) :- write('O').


print_piece(e) :- write('    ').

print_piece(A-B-C-N) :-
        print_symbol(A),
        print_symbol(B),
        print_symbol(C),
        write(N),!.

print_piece(A-B-N) :-
        print_symbol(A),
        print_symbol(B),
        write(N),
        write(' '),!.

print_piece(A-N) :-
        write(' '),
        print_symbol(A),
        write(N),
        write(' '),!.


print_line([M]) :- 
        write('|'),
        print_piece(M),
        write('|').

print_line([A|T]) :-
        write('|'),
        print_piece(A),
        print_line(T).


print_border(Size) :-
        write('   '),
        print_border_aux(Size).
print_border_aux(0).

print_border_aux(Size) :-
        S is Size - 1,
        write(' ____'),
        print_border_aux(S).


print_board_aux([H],Size,Index) :-
        Index < 10,
        print_border(Size), nl,nl,
        write(Index),
        write('  '),
        print_line(H),nl,
        print_border(Size), nl.

print_board_aux([H],Size,Index) :-
        print_border(Size), nl,nl,
        write(Index),
        write(' '),
        print_line(H),nl,
        print_border(Size), nl.

print_board_aux([A|T],Size,Index) :-
        Index < 10,
        print_border(Size), nl, nl,
        write(Index),
        write('  '),
        print_line(A),nl,
        Index2 is Index + 1,
        print_board_aux(T,Size,Index2).

print_board_aux([A|T],Size,Index) :-
        print_border(Size), nl, nl,
        write(Index),
        write(' '),
        print_line(A),nl,
        Index2 is Index + 1,
        print_board_aux(T,Size,Index2).
        
print_horizontal_index(N) :-
        write('     '),
        print_horizontal_index_aux(N,1).


print_horizontal_index_aux(N,N) :-
        write(N),
        nl.

print_horizontal_index_aux(N,Cont) :-
        Cont < 10,
        write(Cont),
        write('    '),
        Cont1 is Cont + 1,
        print_horizontal_index_aux(N,Cont1).

print_horizontal_index_aux(N,Cont) :-
        write(Cont),
        write('   '),
        Cont1 is Cont + 1,
        print_horizontal_index_aux(N,Cont1).


print_board(X) :-
        length(X,Size),
        print_horizontal_index(Size),
        print_board_aux(X,Size,1),nl,!.

%%%%%%%%%%%%%%%% Printing Info %%%%%%%%%%%%%%%%

credits :-
        nl,
        write('This game was developed to the subject PLOG by:'), nl,
        write(' © Rui Pinto - ei11121@fe.up.pt'), nl,
        write(' © Tiago Fernandes -  ei11054@fe.up.pt'),nl.

info :-
        write(' ######    #######  ##     ## ##    ## ##    ## #### '),nl,
        write('##    ##  ##     ## ##     ## ###   ## ##   ##   ##  '),nl,
        write('##        ##     ## ##     ## ####  ## ##  ##    ##  '),nl,
        write('##   #### ##     ## ##     ## ## ## ## #####     ##  '),nl,
        write('##    ##  ##     ## ##     ## ##  #### ##  ##    ##  '),nl,
        write('##    ##  ##     ## ##     ## ##   ### ##   ##   ##  '),nl,
        write(' ######    #######   #######  ##    ## ##    ## #### '),nl,nl.

%Player wins
winner(1) :-
        player(1,Type,_),
        Type = human,
        nl, write('Player 1 won! Congratulations!'), nl.
%Pc wins against a player
winner(1) :-
        player(1, _,Level),
        player(2, Type,_),
        Type = human,
        nl, write('PC 1 won!'), nl, 
        write('Player 2 you\'re beaten by a pc level '),
        write(Level),
        write('! Ahahah'), nl.
%Pc wins against pc
winner(1) :-
        nl, write('PC 1 won!'), nl.

%player wins
winner(2) :-
        player(2,Type,_),
        Type = human,
        nl, write('Player 2 won! Congratulations!'), nl.
%Pc wins against a player
winner(2) :-
        player(2, _,Level),
        player(1, Type,_),
        Type = human,
        nl, write('PC 2 won!'), nl, 
        write('Player 1 you\'re beaten by a pc level '),
        write(Level),
        write('! Ahahah'), nl.
%Pc wins against pc
winner(2) :-
        nl, write('PC 2 won!'), nl.


%%%%%%%%%%%%%%%% Creating Board %%%%%%%%%%%%%%%%

create_board(Size, Board) :-
        M is Size mod 2,
        S is Size/2,
        S1 is truncate(S),
        S2 is (S1 + M) * 2,
        create_board_aux([],S2,Board).


create_board_aux(List, Size, Board) :-
        S is truncate(Size / 2),
        create_board_line(L1,S,2,0,[]),
        append(List,[L1],A),
        create_board_line(L2,S,2,1,[]),
        append(A,[L2],A1),
        EmptyS is Size - 4,
        create_empty_lines(El,EmptyS,S,[]),
        append(A1,El,A2),
        create_board_line(L3,S,1,0,[]),
        append(A2,[L3],A3),
        create_board_line(L4,S,1,1,[]),
        append(A3,[L4],Board).
        
create_board_line(Result,0,_,_,Result).

%type = 0 begins with o, type = 1 begins with x, type = 2 empty
create_board_line(Result, Size, Player, Type, Line) :- 
        Type = 0,
        append(Line,[o-Player,x-Player], L),
        S is Size - 1,
        create_board_line(Result,S,Player,Type, L),!.

create_board_line(Result, Size, Player, Type, Line) :- 
        Type = 1,
        append(Line,[x-Player,o-Player], L),
        S is Size - 1,
        create_board_line(Result,S,Player,Type,L),!.

create_board_line(Result, Size, Player, Type, Line) :- 
        Type = 2,
        append(Line,[e,e], L),
        S is Size - 1,
        create_board_line(Result,S,Player,Type,L),!.


create_empty_lines(Result,0,_,Result). 

create_empty_lines(Result,Cont,Size,List) :-
        create_board_line(R,Size,0,2,[]),
        append(List,[R],M),
        S is Cont - 1,
        create_empty_lines(Result,S,Size,M),!.


%%%%%%%%%%%%%%%% Manipulate Board %%%%%%%%%%%%%%%%

get_element(Element,X,Y,Board) :-
        length(Board, Length),
        X >= 1, X =< Length,
        Y >= 1, Y =< Length,
        get_line(Line, Y, Board),
        get_line_element(Element,X,Line).

get_line(H, 1, [H|_]).
get_line(Line, Y, [_|T]) :-
        Y1 is Y - 1,
        get_line(Line, Y1, T).

get_line_element(Element,1,[Element|_]).
get_line_element(Element, X, [_|T]) :-
        X1 is X - 1,
        get_line_element(Element, X1, T).



set_element(X,Y,E,Board,NewBoard) :-
        length(Board,Size),
        set_element_aux(X,Y,E,1,Size,Board,NewBoard,[]).

set_element_aux(X,Y,E,Y,Size,Board,NewBoard,Aux) :-
        get_line(L,Y,Board),
        set_line_element(X,E,Size,L,N,[]),
        append(Aux,[N],New),
        Cont is Y + 1,
        set_element_aux(X,Y,E,Cont,Size,Board,NewBoard,New).

set_element_aux(_,_,_,Cont,Size,_,NewBoard,NewBoard):-
        Cont > Size.
set_element_aux(X,Y,E,Cont,Size,Board,NewBoard,Aux) :-
        Size >= Cont,
        Cont2 is Cont + 1,
        get_line(L,Cont,Board),
        append(Aux,[L],B2),
        set_element_aux(X,Y,E,Cont2,Size,Board,NewBoard,B2).


set_line_element(1,E,Cont,[_|T],NewLine,Aux) :-
        Cont1 is Cont - 1,
        append(Aux,[E],N),
        set_line_element(0,E,Cont1,T,NewLine,N),!.

set_line_element(_,_,0,_,NewLine,NewLine).
set_line_element(X,E,Cont,[H|T],NewLine,Aux) :-
        X1 is X - 1,
        Cont1 is Cont - 1,
        append(Aux,[H],N),
        set_line_element(X1,E,Cont1,T,NewLine,N).


remove_element(X,Y,Board,NewBoard) :-
        set_element(X,Y,e,Board,NewBoard).



%%%%%%%%%%%%%%%% Play %%%%%%%%%%%%%%%%

/*
play(Origin_x, Origin_y, Destination_x, Destination_y, Board,NewBoard) :-
        length(Board,L),
        Destination_x >= 1,
        Destination_x =< L,
        Destination_y >= 1,
        Destination_y =< L,
        Origin_x >= 1,
        Origin_x =< L,
        Origin_y >= 1,
        Origin_y =< L,
        validate_move(Origin_x, Origin_y, Destination_x, Destination_y, Board,NewBoard).*/

play(Origin_x, Origin_y, Destination_x, Destination_y, Board,Answer) :-
        length(Board,L),nl,
        write(Origin_x),nl,
        write(Origin_y),nl,
        write(Destination_x),nl,
        write(Destination_y),nl,
        write(L),nl,
        Destination_x >= 1,
        Destination_x =< L,
        Destination_y >= 1,
        Destination_y =< L,
        Origin_x >= 1,
        Origin_x =< L,
        Origin_y >= 1,
        Origin_y =< L,
        validate_move(Origin_x, Origin_y, Destination_x, Destination_y, Board,_),
        Answer = ok,!.

play(_, _, _, _, _,'no').

%%%%%%%%%%%%%%%% Moves' Validation %%%%%%%%%%%%%%%%

validate_move(Origin_x, Origin_y, Destination_x, Destination_y, Board,NewBoard) :-
        get_element(Element,Origin_x, Origin_y,Board),
        validate_move_aux(Element,NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        remove_element(Origin_x, Origin_y, Board,B2),
        set_element(Destination_x, Destination_y, NewElement, B2,NewBoard).


validate_move_aux(o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        (Origin_x + 1 =:= Destination_x ; Origin_x - 1 =:= Destination_x) ,
        do_operation_y(N, Origin_y, 1, R),
        R =:= Destination_y,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-N,E2,NewElement).


validate_move_aux(x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x =:= Destination_x,
        do_operation_y(N, Origin_y, 1, R),
        R =:= Destination_y,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-N,E2,NewElement).

validate_move_aux(x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x - 1=:= Destination_x,
        Origin_y =:= Destination_y,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-N,E2,NewElement).

validate_move_aux(x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x + 1=:= Destination_x,
        Origin_y =:= Destination_y,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-N,E2,NewElement).

validate_move_aux(o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(o-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-N,E2,NewElement).

validate_move_aux(o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x + 2 =:= Destination_x,
        do_operation_y(N, Origin_y, 2, R),
        R =:= Destination_y,
        Mid_x is  Origin_x + 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-N,E2,NewElement).

validate_move_aux(o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x - 2 =:= Destination_x,
        do_operation_y(N, Origin_y, 2, R),
        R =:= Destination_y,
        Mid_x is  Origin_x - 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-N,E2,NewElement).

%bouncing case 1
validate_move_aux(o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        length(Board,Length),
        Origin_x =:= Length-1,    
        Origin_x =:= Destination_x,
        do_operation_y(N, Origin_y, 2, R),
        R =:= Destination_y,
        Mid_x is  Origin_x + 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-N,E2,NewElement).

%bouncing case 2
validate_move_aux(o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x =:= 2,    
        Origin_x =:= Destination_x,
        do_operation_y(N, Origin_y, 2, R),
        R =:= Destination_y,
        Mid_x is  Origin_x - 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-N,E2,NewElement).

validate_move_aux(x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(x-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-N,E2,NewElement).

validate_move_aux(x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x =:= Destination_x,
        do_operation_y(N, Origin_y, 2, R),
        R =:= Destination_y,
        Mid_x is  Origin_x,
        do_operation_y(N, Origin_y, 1, Mid_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-N,E2,NewElement).

validate_move_aux(x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x + 2 =:= Destination_x,
        Origin_y =:= Destination_y,
        Mid_x is  Origin_x + 1,
        Mid_y is Origin_y,
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-N,E2,NewElement).

validate_move_aux(x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x - 2 =:= Destination_x,
        Origin_y =:= Destination_y,
        Mid_x is  Origin_x - 1,
        Mid_y is Origin_y,
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-N,E2,NewElement).

validate_move_aux(x-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(x-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-o-N,E2,NewElement).

validate_move_aux(x-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(o-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-o-N,E2,NewElement).

validate_move_aux(x-x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(x-x-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-x-N,E2,NewElement).

validate_move_aux(x-x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x =:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x ,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-x-N,E2,NewElement).

validate_move_aux(x-x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x + 3=:= Destination_x,
        Origin_y =:= Destination_y,
        Mid_x is  Origin_x + 1,
        Mid_y is Origin_y,
        Mid1_x is  Origin_x + 2,
        Mid1_y is Origin_y,
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-x-N,E2,NewElement).

validate_move_aux(x-x-x-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x - 3=:= Destination_x,
        Origin_y =:= Destination_y,
        Mid_x is  Origin_x - 1,
        Mid_y is Origin_y,
        Mid1_x is  Origin_x - 2,
        Mid1_y is Origin_y,
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-x-N,E2,NewElement).


validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(o-o-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x - 3=:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x - 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x - 2,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x + 3=:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x + 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x + 2,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

%bouncing case 1
validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        length(Board,Length),
        Origin_x =:= Length - 2,
        Origin_x + 1 =:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x + 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x + 2,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

%bouncing case 2
validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        length(Board,Length),
        Origin_x =:= Length - 1,
        Origin_x - 1 =:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x + 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

%bouncing case 3
validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x =:= 3,
        Origin_x - 1 =:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x - 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x - 2,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

%bouncing case 4
validate_move_aux(o-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        Origin_x =:= 2,
        Origin_x + 1 =:= Destination_x,
        do_operation_y(N, Origin_y, 3, R),
        R =:= Destination_y,
        Mid_x is  Origin_x - 1,
        do_operation_y(N, Origin_y, 1, Mid_y),
        Mid1_x is  Origin_x,
        do_operation_y(N, Origin_y, 2, Mid1_y),
        get_element(E3,Mid_x,Mid_y,Board),
        E3 = e,
        get_element(E4,Mid1_x,Mid1_y,Board),
        E4 = e,
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(o-o-o-N,E2,NewElement).

validate_move_aux(x-x-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(x-x-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-o-N,E2,NewElement).

validate_move_aux(x-x-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(o-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-x-o-N,E2,NewElement).

validate_move_aux(x-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(o-o-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-o-o-N,E2,NewElement).

validate_move_aux(x-o-o-N, NewElement, Origin_x, Origin_y, Destination_x, Destination_y, Board) :-
        validate_move_aux(x-N,_, Origin_x, Origin_y, Destination_x, Destination_y, Board),
        get_element(E2,Destination_x,Destination_y,Board),
        build_piece(x-o-o-N,E2,NewElement).


deploy_move(A-B-C-N, Xi-Yi-P1-X1-Y1-P2-X2-Y2-P3-X3-Y3, Board, NewBoard) :-
        P1 =\= P2, P2 =\= P3, P1 =\= P3,
        relative_pos(A-B-C,P1,E1),
        relative_pos(A-B-C,P2,E2),
        relative_pos(A-B-C,P3,E3),
        get_element(Eo1, X1, Y1, Board),
        validate_deploy_int(Eo1,N),
        get_element(Eo2, X2, Y2, Board),
        validate_deploy_int(Eo2,N),
        get_element(Eo3, X3, Y3, Board),
        validate_deploy_int(Eo3,N),
        validate_move_aux(E1-N, E1N, Xi, Yi, X1, Y1, Board),
        validate_move_aux(E2-N, E2N, X1, Y1, X2, Y2, Board),
        validate_move_aux(E3-N, E3N, X2, Y2, X3, Y3, Board),
        remove_element(Xi, Yi, Board, NB1),
        set_element(X1, Y1, E1N, NB1, NB2),
        set_element(X2, Y2, E2N, NB2, NB3),
        set_element(X3, Y3, E3N, NB3, NewBoard).

     
deploy_move(A-B-N, Xi-Yi-P1-X1-Y1-P2-X2-Y2, Board, NewBoard) :-
        P1 =\= P2,
        relative_pos(A-B,P1,E1),
        relative_pos(A-B,P2,E2),
        get_element(Eo1, X1, Y1, Board),
        validate_deploy_int(Eo1,N),
        get_element(Eo2, X2, Y2, Board),
        validate_deploy_int(Eo2,N),
        validate_move_aux(E1-N, E1N, Xi, Yi, X1, Y1, Board),
        validate_move_aux(E2-N, E2N, X1, Y1, X2, Y2, Board),
        remove_element(Xi, Yi, Board, NB1),
        set_element(X1, Y1, E1N, NB1, NB2),
        set_element(X2, Y2, E2N, NB2, NewBoard).
        
validate_deploy_int(e, _).
validate_deploy_int(B, N) :-
        B = _-N.



%%%%%%%%%%%%%%%% Operations / Utilities %%%%%%%%%%%%%%%%

do_operation_y(1, O1, O2, R) :-
        R is O1 - O2.

do_operation_y(2, O1, O2, R) :-
        R is O1 + O2.

complex(A-B-_) :-
        atom(A),
        atom(B).
        
relative_pos(A-_-_, Pos, Element) :-
        Pos = 1,
        Element = A.
        
relative_pos(_-B-_, Pos, Element) :-
        Pos = 2,
        Element = B.
relative_pos(_-_-C, Pos, Element) :-
        Pos = 3,
        Element = C.

relative_pos(A-B, Pos, Element) :-
        Pos = 1,
        atom(A),
        atom(B),
        Element = A.
        
relative_pos(A-B, Pos, Element) :-
        atom(A),
        atom(B),
        Pos = 2,
        Element = B.

not(Goal) :- call(Goal),!,fail.
not(_).

build_piece(P1,P2,New) :-
        build_piece_aux(P1,P2,New),!.

build_piece(P1,P2,New) :-
        build_piece_aux(P2,P1,New).


build_piece_aux(X-N, _-G, New) :-
        integer(G),
        integer(N),
        G \= N,
        New = X-N,!.

build_piece_aux(x-N, A-N, New) :-
        atom(A),
        New = x-A-N,!.

build_piece_aux(x-x-N, A-N, New) :-
        atom(A),
        New = x-x-A-N,!.

build_piece_aux(o-N, A-N, New) :-
        atom(A),
        New = A-o-N,!.

build_piece_aux(o-o-N, A-N, New) :-
        atom(A),
        New = A-o-o-N,!.

build_piece_aux(x-o-N, A-N, New) :-
        atom(A),
        New = x-A-o-N,!.

build_piece_aux(S, e, New) :-
        New = S,!.

%%%%%%%%%%%%%%%% Simulations %%%%%%%%%%%%%%%%

simulate_deploy(X, Y, A-B-C-N, Board, NewBoard) :-
        simulate_deploy_aux(X, Y, A-B-C-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-C-N, Board, NewBoard) :-
        simulate_deploy_aux(X, Y, A-C-B-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-C-N, Board, NewBoard) :-
        simulate_deploy_aux(X, Y, B-A-C-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-C-N, Board, NewBoard) :-
        simulate_deploy_aux(X, Y, B-C-A-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-C-N, Board, NewBoard) :-
        simulate_deploy_aux(X, Y, C-A-B-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-C-N, Board, NewBoard) :-
        simulate_deploy_aux(X, Y, C-B-A-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-N, Board, NewBoard) :-
        atom(A), atom(B), integer(N),
        simulate_deploy_aux(X, Y,A-B-N, Board, NewBoard).

simulate_deploy(X, Y, A-B-N, Board, NewBoard) :-
        atom(A), atom(B), integer(N),
        simulate_deploy_aux(X, Y,B-A-N, Board, NewBoard).

%continuar aqui para simular ordem do abcn
simulate_deploy_aux(X, Y, A-B-C-N, Board, NewBoard) :-
        X1 is X - 1,
        do_operation_y(N, Y, 1, Y1),
        simulate_deploy_single(X,Y, X1, Y1, Xr1, Yr1, A-N, Board),
        X2 is Xr1 - 1,
        do_operation_y(N, Yr1, 1, Y2),
        simulate_deploy_single(Xr1,Yr1, X2, Y2, Xr2, Yr2, B-N, Board),
        X3 is Xr2 - 1,
        do_operation_y(N, Yr2, 1, Y3),
        simulate_deploy_single(Xr2,Yr2, X3, Y3, Xr3, Yr3, C-N, Board),
        deploy_move(A-B-C-N, X-Y-1-Xr1-Yr1-2-Xr2-Yr2-3-Xr3-Yr3, Board, NewBoard). 

simulate_deploy_aux(X, Y, A-B-N, Board, NewBoard) :-
        atom(A), atom(B), integer(N),
        X1 is X - 1,
        do_operation_y(N, Y, 1, Y1),
        simulate_deploy_single(X,Y, X1, Y1, Xr1, Yr1, A-N, Board),
        X2 is Xr1 - 1,
        do_operation_y(N, Yr1, 1, Y2),
        simulate_deploy_single(Xr1,Yr1, X2, Y2, Xr2, Yr2, B-N, Board),
        deploy_move(A-B-N, X-Y-1-Xr1-Yr1-2-Xr2-Yr2, Board, NewBoard). 
        

simulate_deploy_single(X, Y, Xf, Yf, Xr, Yr,E,  Board) :-
        validate_move_aux(E, _, X, Y, Xf, Yf, Board),
        Xr is Xf,
        Yr is Yf.

simulate_deploy_single(X, Y, Xf, Yf, Xr, Yr, E,  Board) :-
        Xf < X + 1,
        Xf1 is Xf + 1,
        simulate_deploy_single(X, Y, Xf1, Yf, Xr, Yr, E,  Board).
     
%%%%%%%%%%%%%%%% Game's Cycle %%%%%%%%%%%%%%%%        
game_cycle(B, 1) :-
        player_move(1,B,NB),
        not(check_end_of_game(NB)),
        player_move(2,NB,NNB),
        not(check_end_of_game(NNB)),
        game_cycle(NNB, 1).

game_cycle(B, 2) :-
        player_move(1,B,NB),
        not(check_end_of_game(NB)),
        player(2,_,Dif),
        pc_move(2,Dif,NB,NNB),
        not(check_end_of_game(NNB)),
        game_cycle(NNB, 2).

game_cycle(B, 3) :-
        player(1,_,Dif),
        pc_move(1,Dif,B,NB),
        not(check_end_of_game(NB)),
        player_move(2,NB,NNB),
        not(check_end_of_game(NNB)),
        game_cycle(NNB, 3).

game_cycle(B, 4) :-
        player(1,_,Dif),
        pc_move(1,Dif,B,NB),
        not(check_end_of_game(NB)),
        player(2,_,Dif2),
        pc_move(2,Dif2,NB,NNB),
        not(check_end_of_game(NNB)),
        game_cycle(NNB, 4).
      

game_cycle(_, _) :- 
        nl, write('End of Game'),nl, credits.

%%%%%%%%%%%%%%%% Doing Moves %%%%%%%%%%%%%%%%

pc_move(N,1,B,Answer) :-
        get_list_of_moves(N,B, Moves),
        pc_move_random(Moves, [Xi,Yi,Xf,Yf]),
        Answer = Xi-Yi-Xf-Yf.

pc_move(N,2,B,Answer) :-
        best_move(N,B, Moves),
        pc_move_random(Moves, [Xi,Yi,Xf,Yf]),
        Answer = Xi-Yi-Xf-Yf.

pc_move(N,3,B,NB) :-
        write('PC '),
        write(N),
        write(' turn. '), nl,
        write('waiting...'), nl,
        best_move(N,B, Moves),
        pc_move_random(Moves, [Xi,Yi,Xf,Yf]),
        pc_move_aux(Xi-Yi-Xf-Yf, N, B, NB),!.


%not complex used for level 3
pc_move_aux(Xi-Yi-Xf-Yf, N, B, NB) :-
        get_element(Element,Xi,Yi,B),
        not(complex(Element)),
        player_move_aux(N, Xi-Yi-Xf-Yf, B, NB),
        sleep(3),
        write(' - I want to move from '),
        write(Xi-Yi),
        write(' to '),
        write(Xf-Yf), nl,nl, 
        print_board(NB),!.

%complex to deploy
pc_move_aux(Xi-Yi-_-_, _, B, NB) :-
        random(0,100,Rand),
        Rand < 20, %probablility of 20% to do deploy
        get_element(Element,Xi,Yi,B),
        simulate_deploy(Xi, Yi, Element, B, NB),
        sleep(3),
        write(' - I want to deploy the piece '),
        write(Xi-Yi), nl, nl,
        print_board(NB),!.

%Complex not to deploy
pc_move_aux(Xi-Yi-Xf-Yf, N, B, NB) :-
        player_move_aux(N, Xi-Yi-Xf-Yf, B, NB),
        sleep(3),
        write(' - I want to move from '),
        write(Xi-Yi),
        write(' to '),
        write(Xf-Yf), nl, nl, 
        print_board(NB),!.

pc_move_random(Moves, Chosen) :-
        length(Moves,Length),
        random(0, Length, N),
        pc_move_random_aux(N,Moves, Chosen).
        
pc_move_random_aux(0, [H|_], H).
pc_move_random_aux(Cont, [_|T], Chosen) :-
        Cont1 is Cont - 1,
        pc_move_random_aux(Cont1, T, Chosen).

player_move(N,B,NB) :-
        write('Player '),
        write(N),
        write(' turn: Xi-Yi-Xf-Yf  /  deploy.'),nl,
        read(Text),
        player_move_aux(N, Text, B, NB),
        print_board(NB),!.

player_move(N,B,NB) :-
        write('Not allowed'), nl,
        player_move(N,B,NB).

player_move_aux(N, Read,B,NB) :-
        X1-Y1-X2-Y2 = Read,
        integer(X1), integer(Y1), 
        integer(X2), integer(Y2),
        get_element(E,X1, Y1, B),
        E = _-N, 
        play(X1, Y1,X2,Y2,B, NB).

player_move_aux(N, Read,B,NB) :-
        'deploy' = Read,
        write('OriginX-Origin-Y'),nl,
        read(X1-Y1),
        integer(X1), integer(Y1),
        get_element(E,X1, Y1, B),
        %write(E),nl,
        E = _-_-N,
        play_deploy(E,X1-Y1,B,NB).
             

play_deploy(A-B-C-N, Coord0,Board, NewBoard) :-
        write('P1-X1-Y1-P2-X2-Y2-P3-X3-Y3'),nl,
        read(P1-X1-Y1-P2-X2-Y2-P3-X3-Y3),
        Tx = Coord0-P1-X1-Y1-P2-X2-Y2-P3-X3-Y3,
        deploy_move(A-B-C-N, Tx, Board, NewBoard),!.

play_deploy(A-B-N, Coord0, Board, NewBoard) :-
        atom(A), atom(B), integer(N),
        write('P1-X1-Y1-P2-X2-Y2'),nl,
        read(P1-X1-Y1-P2-X2-Y2),
        Tx = Coord0-P1-X1-Y1-P2-X2-Y2,
        deploy_move(A-B-N,Tx , Board, NewBoard),!.


        
%%%%%%%%%%%%%%%% Checking game termination %%%%%%%%%%%%%%%%

check_end_of_game(B,Answer) :-
         get_line(L1, 1, B),
         check_end_of_game_aux(L1,1),
         Answer=1,!.
      
check_end_of_game(B,Answer) :-
         length(B,N),
         get_line(L2, N, B),
         check_end_of_game_aux(L2,2),
         Answer=2,!.

check_end_of_game(B,Answer) :-
         check_end_of_game_nopieces(B,1), 
         Answer=2,!.

check_end_of_game(B,Answer) :-
         check_end_of_game_nopieces(B,2),
         Answer=1,!.

check_end_of_game(_,'no').

   
   
check_end_of_game_aux([_-N|_],N).

check_end_of_game_aux([_|T],N) :-
        check_end_of_game_aux(T,N).

check_end_of_game_nopieces([], _).
check_end_of_game_nopieces([H|T], N) :-
        check_end_of_game_nopieces_aux(H,N),
        check_end_of_game_nopieces(T,N).
        
check_end_of_game_nopieces_aux([],_).
check_end_of_game_nopieces_aux([H|_], N) :-
        H = _-N, !, fail.
check_end_of_game_nopieces_aux([_|T], N) :-
        check_end_of_game_nopieces_aux(T,N).

        
        

%%%%%%%%%%%%%%%% Getting game possibilities: moves/pieces%%%%%%%%%%%%%%%%

get_list_of_moves(N,Board, List) :-
        get_player_pieces_position(N,1,1,Board,[],L),
        possible_moves(L,Board, [], List).

get_player_pieces_position(N, X, Y, Board, Acum, List) :-
        length(Board,Length),
        X =:= Length,
        Y =:= Length,
        player_pieces_position_aux(N, X, Y, Board, Acum, List),!.

get_player_pieces_position(N, X, Y, Board, Acum, List) :-
        length(Board,Length),
        X =:= Length,
        player_pieces_position_aux(N, X, Y, Board, Acum, New),
        X1 is 1,
        Y1 is Y + 1,
        get_player_pieces_position(N,X1,Y1,Board, New, List).

get_player_pieces_position(N, X, Y, Board, Acum, List) :-
        player_pieces_position_aux(N, X, Y, Board, Acum, New),
        X1 is X + 1,
        get_player_pieces_position(N,X1,Y,Board, New, List).
        
player_pieces_position_aux(N, X, Y, Board, List, Return) :-
        get_element(E, X, Y, Board),
        E = _-N,
        append(List, [[X,Y,E]] , Return).

player_pieces_position_aux(_, _, _, _, List, Return) :-
        Return = List.

possible_moves([],_, Acum, List) :-
        List = Acum.
possible_moves([H|T],Board, Acum, List) :-
        possible_moves_aux(H, Board, Return),
        append(Acum, Return, New),
        possible_moves(T, Board, New, List).
    

possible_moves_aux(H, Board, Return) :-
        [X,Y,E] = H,
        generate_moves(E, X, Y, Board, Return).


generate_moves(E,X,Y, Board, Moves) :-
        Xf is X - 3,
        Yf is Y - 3,
        generate_moves_aux(E, X, Y, Xf, Yf, Board, [], Moves).

%base case
generate_moves_aux(E, Xi, Yi, Xf, Yf, Board, Acum, List) :-
        Xf =:= Xi + 3,
        Yf =:= Yi + 3,
        generate_moves_validator(E,Xi,Yi,Xf,Yf,Board, Acum, New),
        List = New.
        
%change line and column
generate_moves_aux(E, Xi, Yi, Xf, Yf, Board, Acum, List) :-
        Xf >= Xi + 3,
        generate_moves_validator(E,Xi,Yi,Xf,Yf,Board, Acum, New),
        Xf1 is Xi - 3,
        Yf1 is Yf + 1, 
        generate_moves_aux(E, Xi, Yi, Xf1, Yf1, Board, New, List).

%change column
generate_moves_aux(E, Xi, Yi, Xf, Yf, Board, Acum, List) :-
        generate_moves_validator(E,Xi,Yi,Xf,Yf,Board, Acum, New),
        Xf1 is Xf + 1,
        generate_moves_aux(E, Xi, Yi, Xf1, Yf, Board, New, List).

generate_moves_validator(E,Xi,Yi,Xf,Yf,Board, Acum, New) :-
        validate_move_aux(E, _, Xi, Yi, Xf, Yf, Board),
        append(Acum,[[Xi,Yi,Xf,Yf]], New).

generate_moves_validator(_, _, _, _, _, _, Acum, New) :-
        New = Acum.

%%%%%%%%%%%%%%%% BEST MOVE CALCULATION %%%%%%%%%%%%%%%%

best_move(N, Board, Moves) :-
        get_list_of_moves(N, Board, List),
        Oponent is (N mod 2) + 1,
        get_list_of_moves(Oponent, Board, Oponent_moves),
        best_move_points(List, Board, N, Oponent_moves, -500, Highest, [], Pmoves),
        best_move_moves(Highest, Pmoves, [], Moves).


best_move_points([], _, _, _, Highest, Highest, Moves, Moves).
                                   
best_move_points([H|T], Board, N, Oponent_moves, Current, Highest, Acum, Moves) :-
        get_move_points(H, Board, N, Oponent_moves, P), !,
        best_move_points_aux(H, P, Current, NP, Acum, New),
        best_move_points(T, Board, N, Oponent_moves, NP, Highest, New, Moves).

best_move_points_aux(H, P, Current, NP, Acum, New) :-
        P > Current,
        [A,B,C,D] = H,
        NP is P,
        append(Acum,[[A,B,C,D,P]], New).

best_move_points_aux(H, P, NP, NP, Acum, New) :-
        [A,B,C,D] = H,
        append(Acum,[[A,B,C,D,P]], New).
        
                               
best_move_moves(_, [], Moves, Moves).

best_move_moves(Highest, [H|T], Acum, Moves) :-
        [A,B,C,D, P] = H,
        P =:= Highest,
        append(Acum, [[A,B,C,D]], M1),
        best_move_moves(Highest, T, M1, Moves).

best_move_moves(Highest, [_|T], Acum, Moves) :-
        best_move_moves(Highest, T, Acum, Moves).

best_move_selec([H|_], 0, H).
best_move_selec([_|T], Index, Move) :-
        I is Index - 1,
        best_move_selec(T, I, Move).

%%%%%%%%%%%%%%%% Move Points Calculation %%%%%%%%%%%%%%%%

get_move_points([Xi,Yi,Xf,Yf], Board, N, Oponent_moves,Points) :-
        length(Board, Length),
        get_element(Ei, Xi, Yi, Board),
        get_element(Ef, Xf, Yf, Board),
        build_piece(Ef, Ei, E2),
        Oponent is (N mod 2) + 1,
        get_move_points_aux(N, Length, Yf, P1), %points due to the proximity of the end
        add_points(Ef,Oponent,P2), %points due to eat an oponent piece
        analyse_oponent_moves(Xf, Yf, E2, Oponent_moves, P3), %points due to oponent possibility to eat
        analyse_in_danger(Xi, Yi, Oponent_moves, Ei, P4), %points from running away
        random(0,100,R),
        analyse_jump(R,Ei, E2, P5), %points to build a compost piece
        Points is P1 + P2 + P3 + P4 + P5.
        
get_move_points_aux(1, _, 1, Points) :- 
        Points is 100.              
get_move_points_aux(1, Length, Yf, Points) :- 
        Points is (Length + 1 - Yf) * 3.

get_move_points_aux(2, Yf, Yf, Points) :- 
        Points is 100.
get_move_points_aux(2, _, Yf, Points) :- 
        Points is Yf * 3.

analyse_jump(R, Ei, E2, Points) :-
        R < 10, %with a probability of 10% it incentivates to jump to another of his pieces
        Ei = _-N,
        E2 = _-N,
        Ei \= E2,
        Points is 4.

analyse_jump(_,_,_,0).

 %check if a piece is about to be eaten if not moved, if so, incentivate to move
analyse_in_danger(Xi, Yi, List, E, Points) :-
        analyse_in_danger_aux(Xi, Yi, List, E, 0 , Points).

analyse_in_danger_aux(_, _, [], _, Points, Points).

analyse_in_danger_aux(Xi, Yi, [H|T], E, Acum, Points) :-
        H = [_, _, Xf, Yf],
        Xi =:= Xf,
        Yi =:= Yf,
        add_points(E,_,V),
        V1 is truncate(V / 10),
        V2 is Acum + V - V1, %if it can run away or eat chooses to eat
        analyse_in_danger_aux(Xi, Yi, T, E, V2, Points).

analyse_in_danger_aux(Xi, Yi, [_|T], E, Acum, Points) :-
        analyse_in_danger_aux(Xi, Yi, T, E, Acum, Points).
        
%List has oponents moves      
analyse_oponent_moves(Xf,Yf, MyE, List, Points) :-
        analyse_oponent_moves_aux(Xf, Yf, List, MyE, 0, Points),!.


analyse_oponent_moves_aux(_, _, [], _, Acum, Points) :-
        A1 is Acum / 1.2,
        A2 is truncate(A1),
        Points is 0 - A2.
              
analyse_oponent_moves_aux(Xf,Yf,[H|T], E, Acum, Points) :-
        H = [_, _, Xf1, Yf1],
        Xf =:= Xf1,
        Yf =:= Yf1,
        add_points(E,_,V),
        Value is Acum + V,
        analyse_oponent_moves_aux(Xf, Yf, T, E, Value, Points).

analyse_oponent_moves_aux(Xf,Yf,[_|T], E, Acum,Points) :-
        analyse_oponent_moves_aux(Xf, Yf, T, E, Acum, Points).

        
 add_points(E, N, Points) :-
        E = _-_-_-N,
        Points is 30.
        
 add_points(E, N, Points) :-
        E = _-_-N,
        Points is 20.
        
 add_points(E, N, Points) :-
        E = _-N,
        Points is 10.

 add_points(_, _, Points) :-
        Points is 0.
 
%%%%%%%%%%%%%%%% Game's initializer %%%%%%%%%%%%%%%%
                        
init :-
        info,
        choose_mode(M),
        write('This game has a square even board, \nwhich size do you want? (size > 5)'), nl,
        read(Size),
        integer(Size),
        Size > 5,
        create_board(Size,B),
        print_board(B),
        game_cycle(B,M).

 user:runtime_entry(start) :-
        server.
