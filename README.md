# binary_compiler
A tool that allows you to turn ascii 0/1 into actual 0/1.<br>
To run, you must first compile using:
```
make
```
Then run with:
```
./binary_compiler <file_to_compile> (optional: output file's name)
```

You can add comments using ``//`` and you can use any amount of whitespaces between bits.<br>
Any other characters outside of comments scope will raise an error.
