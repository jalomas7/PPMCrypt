# PPMCrypt
command line program that can perform a basic OR-based stegnographic encryption and decryption using .ppm image files.
All source files are written in C++, and an example ppm image is included.

The security of this program comes from both the key used, and the image itself. If one key is used to encrypt a message in one image, that same key will not decrypt the same message in another image (provided that second image did not use the same key for encryption). since each key is represented as an integer value, each encryption per unique image has 2^32 possible keys.

one could easily expand this program to any image format by using an image format converter such as the linux "convert" program.

To compile, just download all files to a directory, navigate to that directory, and use make. Usage instructions can be seen by executing without arguments. 

This project was originally a homework problem assigned by Dr. Jens Gregor of the University of Tennessee, Knoxville. Do not plagiarize. 
