function initModalWindow(openButtonId, closeButtonId, modalWindowId) {
  var openButton = document.getElementById(openButtonId);
  var closeButton = document.getElementById(closeButtonId);

  openButton.onclick = function() {
    document.getElementById(modalWindowId).style.display = "block";
    document.onkeydown = function(evt) {
        evt = evt || window.event;
        if (evt.keyCode == 27) {
          document.getElementById(modalWindowId).style.display = "none";
        }
    };
  }
  closeButton.onclick = function() {
    document.getElementById(modalWindowId).style.display = "none";
  }
  window.onclick = function(event) {
    if (event.target == document.getElementById(modalWindowId)) {
      document.getElementById(modalWindowId).style.display = "none";
    }
  }
}