-module(test).
-export([fac/1, hello/0, add/2, greet_and_add_two/1, oh_god/1, old_enough/1, duplicate/2, tail_duplicate/2, tail_duplicate/3, reverse/1, tail_reverse/1, tail_reverse/2]).

fac(0) -> 1;
fac(N) -> N * fac(N-1).

hello() -> 
    io:format("hello, world!~n").

add(X, Y) ->
    X + Y.

greet_and_add_two(X) ->
    hello(),
    add(X,2).

oh_god(N) ->
    if N =:= 2 -> might_succeed;
        true -> always_does
    end.

old_enough(X) when X >= 16 -> true;
old_enough(_) -> false.

duplicate(0,_) ->
    [];
duplicate(N, Term) when N > 0 ->
    [Term|duplicate(N-1,Term)].
tail_duplicate(N, Term) ->
    tail_duplicate(N, Term, []).
tail_duplicate(0, _, List) ->
    List;
tail_duplicate(N, Term, List) when N > 0 ->
    tail_duplicate(N-1, Term, [Term|List]).

reverse([]) -> [];
reverse([H|T]) -> reverse(T)++[H].

tail_reverse([H|T]) ->
    tail_reverse([H|T], []).
tail_reverse([], List) ->
    List;
tail_reverse([H|T], List) ->
    tail_reverse(T, [H]++List).
