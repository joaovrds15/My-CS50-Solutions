var i = 0;
var text = "Hey, I'm João Victor ;)";

function typing(){
    if(i < text.length){
        document.getElementById("name").innerHTML += text.charAt(i);
        i++;
        setTimeout(typing,150);
    }
}
typing();