This project uses Pyodide. See third-party/pyodide/LICENSE.

To run `demo` inside a terminal: `python3 -m demo`.
To run `demo` in a browser: `python3 -m http.server` and go to `http://localhost:8000/demo`.

PUI provides a 16 by 32 character screen area with PICO-8 colors. Define a function which uses `pui.btnp` to read input and `pui.cset` to draw and pass it to `pui.run`.
