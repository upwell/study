#!/usr/bin/env ruby

#
# random number guess game
#

is_1st_time = true
is_quit_game = false

while true
    if not is_1st_time
        puts " === Game restarted ==="
    else
        is_1st_time = false
    end

    dest_num = rand(10)

    while true
        puts "Input a integer between 0 ~ 9 or ctrl+D to quit: "
        input_num = gets

        if input_num.nil?
            is_quit_game = true
            break
        end

        input_num = input_num.to_i
        if input_num == dest_num
            puts "You got it."
            break
        elsif input_num > dest_num
            puts "You inputted a larger integer. Try again."
        else
            puts "You inputted a smaller integer. Try again."
        end
    end

    if is_quit_game
        break
    end
end

