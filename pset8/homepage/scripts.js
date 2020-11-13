




function greet()
{
    let name=document.querySelector("#name").value;  /* look for somenthing with an id of name */

    if (name === '')
    {
        name = "world";
    }
    /*alert("hello, " + name + "!");*/
    document.querySelector('#pool').innerHTML = name + "Stonk";
}



var acc = document.getElementsByClassName("accordion");
var i;

for (i = 0; i < acc.length; i++) {
  acc[i].addEventListener("click", function() {
    this.classList.toggle("active_faq");
    var panel = this.nextElementSibling;
    if (panel.style.maxHeight) {
      panel.style.maxHeight = null;
    } else {
      panel.style.maxHeight = panel.scrollHeight + "px";
    }
  });
}




// var prevScrollpos = window.pageYOffset;
// window.onscroll = function() {
// var currentScrollPos = window.pageYOffset;
//   if (prevScrollpos > currentScrollPos)  /* scroll up  --> show */
//   {
//     document.getElementById("navbar_1").style.top = "0";
//   }
//   else if (prevScrollpos < currentScrollPos && window.pageYOffset < 20)  /* scroll down but on the top  --> show */
//   {
//     document.getElementById("navbar_1").style.top = "0";
//   }
//   else                                                              /* scroll up --> hide */
//   {
//     document.getElementById("navbar_1").style.top = "-28em";
//   }
//   prevScrollpos = currentScrollPos;
// }


// var prevScrollpos = window.pageYOffset;
// window.onscroll = function() {
// var currentScrollPos = window.pageYOffset;
//   if (prevScrollpos > currentScrollPos && window.pageYOffset < 100)  /* scroll up  --> show */
//   {
//     document.getElementById("navbar_1").style.top = "0";
//   } else                                                              /* scroll up --> hide */
//   {
//     document.getElementById("navbar_1").style.top = "-28em";
//   }
//   prevScrollpos = currentScrollPos;
// }