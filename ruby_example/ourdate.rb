#!/usr/bin/env ruby

$months31 = [1,3,5,7,8,10,12]
$months30 = [4,6,9,11]

class OurDate
    attr_accessor :year
    attr_accessor :month
    attr_accessor :day

    def initialize(year, month, day)
        @year = year
        @month = month
        @day = day
    end

    def is_equal?( d )
    	puts @year == d.year and @month == d.month and @day = d.day
    end

    def is_leap_year?
    	if @year % 400 == 0
	    puts 1
	elsif @year % 100 == 0
	    puts nil
	elsif @year % 4 == 0
	    puts 1
	else
	    puts nil
	end
    end

    def check_month
        if @month == 13
	    @month = 1
	    @year = @year + 1
	elsif @month == 0
	    @month = 12
	    @year = @year - 1
	end
    end

    def tomorrow
    	@day = @day + 1
	if @day > 31
	    for i in $months31
	        if @month == i
		    @day = 1
		    @month = @month + 1
		    check_month
		end
	    end
	elsif @day > 30
	    for i in $months30
	        if @month == i
		    @day = 1
		    @month = @month + 1
		    check_month
		end
	    end
	elsif @day > 28 and @month == 2
	    @day = 1
	    @month = @month + 1
	    check_month
	end
    end

    def yesterday
        @day = @day - 1
        if @day == 0
            @month = @month - 1
	    check_month
            for i in $months31
	        if @month == i
	            @day = 31
	        end	    
	    end
	    for i in $months30
	        if @month == i
	            @day = 30
	        end
	    end
	    if @month == 2
	        @day = 28
	    end
	end
    end

    def forward_time(n)
        for i in 0..n
            tomorrow
	end
    end

    def reverse_time(n)
        for i in 0..n
	    yesterday
        end
    end

    def what_day
    	puts "Today is #{month}/#{day}, #{year}!"
    end
end

