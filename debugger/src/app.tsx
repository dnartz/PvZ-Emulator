import * as React from 'react';
import * as ReactDOM from 'react-dom';
import { MainFrame } from "./MainFrame"

window.addEventListener("load", function () {
    let app = document.createElement("div");
    app.id = "app";

    document.getElementsByTagName("body")[0].appendChild(app);
    ReactDOM.render(<MainFrame/>, app);
});
