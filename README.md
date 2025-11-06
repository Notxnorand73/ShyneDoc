# ShyneDoc
Website of the documentation for the Shyne programming language.

Full documentation:

Shyne V.1.4
Shyne is a high level programming language for beginner programmers. It contains simple grammar and is easy to use.
It is for anyone to be able to code while keeping experienced programmers able to create anything.
Below are the instructions for the language.
    Shyne files have the .shy file extension.
Commands

out(), print to the console with a newline:
    out("Hello, World!")

in(),  Get an input from the user with a newline:
    in("Enter some text: ")

repeat a:b, repeats through an integer or array "a" with "b" and starts with b=1. raises an IndexError if a is less than 0
repeat 10:i #value
    out(i) #prints 1 2 3 4…10
end
repeat [1, 5, 2]:i #value
    out(i) #prints 1 2 3
end
repeat i in [1, 5, 2] #index
    out(i) #prints 1 5 2
end

if _ elif _ else, a standard if chain:
if 1=3
    out("1 is equal to 3")
elif 1=2
    out("1 is equal to 2")
else
    out("1 is not 2 nor 3")
end

while _, a standard while loop, iterating over a block of code until the condition is false:
a:1
while a<10
    out(a)
    a:a+1
end

skip/break, loop controls:
repeat 10:i
    if i = 5
        skip
    end
    out(i)
    if i = 8
        break
    end
end #outputs 1 2 3 4 6 7 8

a:_, Variable declaration:
text:"Some text"
num:2.31
#inline
out(a, a:10-3) #outs 7

func a(b), a function definition:
func say(text)
    out(text)
end

say("Hello, World!")

#Inline functions

func say(text) => out(text)

say("Hello, World!")

send, passes the value out of the function as its return value:
func lengthof(text)
    send text.length
end

out(lengthof("Hello, World!"))

local, variable globality. variables declared at a level are automatically defined in every level above unless it is defined as local:
func lengthof(text)
    local textlength:text.length
    send textlength
end

out(lengthof("Hello, World!"))

class, a class blueprint

class Animal
    func init(name)
        self.name:name
    end
    func greet()
        out(i"My name is {self.name}!")
    end
end

dog:Animal.new("Max")
dog.greet #outputs My name is Max!

try … error _ … end, a try-error block for error catching:
try
    A:1/0
error ZeroDivision
    out("Division by 0!")
end

Errors include
TypeError:
When a command expects a certain type but receives a different type.
try
    out("1"…10)
error TypeError
    out("Error")
end
When two different types concatenate.
try
    A:"Hello" & 1
error TypeError
    out("Error")
end

ConversionError:
When an object cannot be converted to another type:
try
    A:"Not a number".num
error ConversionError
    out("Error")
end

IndexError:
When the index of a list is out of bounds
a:[1,2,3,4,5]
try
    out(a{10})
error IndexError
    out("Error")
end

ZeroDivision:
When dividing by zero
try
    a:1/0
error ZeroDivision
    out("Error")
end

ConditionalError:
When a logical condition is given a non-Boolean value
try
    if "A" and 0
        out("Text")
error ConditionalError
    out("Error")
end
ValueError:
When an expression is undefined
try
    sqrt:-1^0.5
error ValueError
    out("ERROR")
end

raise, An error raiser, can be given a custom name
try
    raise CustomError
error CustomError
    out("ERROR")
end

[], Arrays:
[1, 5, 3, 8] #list
["One"=>1, "Two"=>2] #dictionary
array{index} #gets the object at an index of an array, the first element starts at 1
array{key}' #the apostrophe states that the "{}" is a key rather than an index. returns the value
setindex(value) #changes the indexing to start at value instead of 1. can be used to set the indexing to 0 for those who have experience with programming:
arr{[0,1,2]} #this would normally be 1,2,3 but the id(0) makes the 1,2,3 into a 0,1,2
[for var in iterable, if condition, expression, elif, expression,...]
#var → iteration variable
#iterable → array or range
#if condition → condition for var
#expression → value to append when condition is true

Methods:
.split(a) #split a text using a delimiter: "An A".split(" ") = ["An", "A"]. if the delimiter is blank, it separates every character.
.down #converts text to minuscule: "TEXT".down = "text"
.up #converts text to majuscule: "text".up = "TEXT"
.shiftr #shifts array indices right: [2,3,4,5,6,7,8].shiftr = [3,4,5,6,7,8,2]
.shiftl #shifts array indices left: [3,4,5,6,7,8,2].shiftl = [2,3,4,5,6,7,8]
.indexof(a) #gets the index of an object in an array with 1-based indexing:["Apple", "Orange", "Banana"].indexof("Apple") = 1
.length #gets the length of an object: "Four".length = 4
.num #converts an object to type num: "18".num = 18
.string #converts an object to type string: 18.string = "18"
.add(a) #adds a value to an array: [1,3,5,7].add(9) = [1,3,5,7,9]
.remove(index) #removes the object at the index of an array: [1,3,5,7,9].remove(3) = [1,3,7,9]
array.map(a, b) #a → dictionary mapping original values → new values, b → default value if no key matches (optional)

Data types:
true/false #Boolean
"string" #string literal
>'
Multiline string literal
'<
123 #number literal
[] #array literal
#a comment
#Shyne is also Dynamically typed
typeof(x) #returns "str" "num" "bool" or "arr"

Operators:
not #logical negation.
and #logical and.
or #logical or.
+ #plus operator.
- #minus operator.
* #multiplicative operator.
^ #exponentiation operator.
/ #division operator.
% #modulo/remainder operator.
< #less than comparison.
> #greater than comparison.
= #equality comparison.
<= #less than or equality comparison
>= #greater than or equality comparison
!= #inequality comparison

Other:
random() #picks a random number from a given array and returns it
... #integers between, returns a list of integers between two numbers including the two numbers: 1…5 = [1,2,3,4,5], 5...1 = [5,4,3,2,1]
i"Text {value}." #interpolated string
asccode(x) #Converts a string → ASCII codes (number/array)
out(asccode("A")) # → 65
out(asccode("10")) # → [49, 48]
ascchar(x) #Converts number → ASCII character
out(ascchar(65)) # → "A"
out(ascchar([49, 48])) # → "10"
unicode(x) #Converts character or string → Unicode code points
out(unicode("A"))       # → 65
out(unicode("Hello"))   # → [72,101,108,108,111]
out(unicode("😊"))      # → [128522]
unichar(x) #Converts Unicode code point or array → character(s)
out(unichar(65))                     # → "A"
out(unichar([72,101,108,108,111]))  # → "Hello"
out(unichar([128522,128512]))       # → "😊😀"

Rules:
Concatenation is a defined as ("It is the number " & 7.string) or ("It is " & "9:00!")
method chaining goes from left to right "Hello, World!".split(" ").length returns "Hello, World!" to ["Hello,", "World!"] to 2
Order of operations is PEMDMAS:()^*/%+-

File management/Libraries
Outside files:
download "lib_name" #library must be stored on the system
download "lib_name" as name #saves the library as the name
#Library commands are defined by: lib.command
exists filepath #returns true or false
take filepath #retrieves the data from a file, can have a delimiter
enter filepath #replaced the data in a file
delete filepath #deletes a file
add filepath #makes a file
duplicate filepath #copies a file
addto filepath #appends data to a file

In many other languages like Python, you would need to import a separate module to handle file operations. Shyne includes these commands built-in to simplify basic tasks for beginners

Library creation:
Library files have the .shyl extension meaning .shylibrary or .dll. shyl files are the front end while the dll is the backend. these can also be referred to as stars
file.data:["name":"", "creator":"", "desc":""] #library data for name, creator and description. Defined at the top of the code
file.a(b) is the library function definition. These can stack to form functions with many attributes:
file.create() #layer 1
    file.screen(a,b) #layer 2
        out(i"Screen made with dimensions {a}, {b}!")
    end
    out("ERROR")
end
#after downloading the library
file.create.screen(600, 800) #outputs Screen made with dimensions 600, 800! the Ex layer doesnt run, it only runs the top level
file.create() #outputs ERROR the screen layer doesnt run



Tips
Clear names:
Player_Score is better than ps. Player_Score easily tells the programmer what the variable is for. ps doesnt.
Polishing:
make indentations to know which end goes to which block

External links:
Lua: https://www.lua.org
Python: https://www.python.org
Ruby: https://www.ruby-lang.org/en/
Programming Language: Shyne, Shine/Light
Program Files: Suns, .shy
Libraries: Stars, .shyl
Methods: Fos, greek
Functions: Lumens, Latin
