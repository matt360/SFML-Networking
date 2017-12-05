# SFML-Networking

To build the application you will need to download SFML 2.4.2 for Visual Studio 2015 32-bit (https://www.sfml-dev.org/files/SFML-2.4.2-windows-vc14-32-bit.zip) 

and place it in the same folder as the VS solution file.

Start the application. Choose if you want to be the `server` or the `client` (`client`s started before the `server` will not be able to connect).

First `client` will connect automatically. If you want to connect more `client`s: run another instance of the application, choose to be a `client`, press `C` on the `server`'s side to connect the new `client`.

`server` input:

`D` - display socket messages in the terminal.

`L` - toggle lerp mode (on/off).

`P` - toggle prediction mode (linear prediction/quadratic prediction).

`C` - establish a connection with a new client.

`R` - restart player's alive state.

`client` input:

`D` - display socket messages in the terminal.
