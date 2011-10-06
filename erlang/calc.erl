-module(calc).
-export([rpn/1]).

rpn(L) when is_list(L) ->
    [Res] = list:foldl(fun rpn/2, [], string:tokens(L, " ")),
    Res.

rpn(X, Stack) ->
    [read(X)|Stack].

read(X) ->
    case string:to_float(X) of
        {error, no_float} -> list_to_integer(X);
        {F, _} -> F
    end.

