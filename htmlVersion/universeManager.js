class UniverseManager {
    quads = new Array()
    generation = 0;
    generations = new Array()
    normalisationMarker = false;
    constructor(level) {
        this.#createQuads(level)
        this.updateGeneration()        
    }
    getUniverseString(){
        let left = this.size - 1;
        let right = 0;
        let top = this.size - 1;
        let bot = 0;
        let marker = false;
        this.rootLevel = this.level;
        
        for (let y = 0; y < this.size; y++) {            
            for (let x = 0; x < this.size; x++) {                
                if (this.quads[y * this.size + x].value == 1) {
                    marker = true;
                    left = left < x ? left : x; 
                    right = right > x ? right : x;
                    top = top < y ? top : y;
                    bot = bot > y ? bot : y;
                }                
            }            
        }

        let rootSize = 0;
        if (marker) {
            for (let l = 2; l < this.level; l++) {
                rootSize = Math.pow(2, l);
                if((right - left + 1) <= rootSize && (bot - top + 1) <= rootSize) {
                    this.rootLevel = l;
                    break;
                }            
            }        
        }
        else {
            right = this.size - 1;
            left = 0;
            bot = this.size - 1;
            top = 0;
            rootSize = this.size
        }

        let str = "";
        for (let y = top; y < top + rootSize; y++) {
            for (let x = left; x < left + rootSize; x++) {                               
                str += this.quads[y * this.size + x].value.toString()            
            }
            str += ";"
        }      
                
        return str;
    }
    #createQuads(level) {
        let quadStr = ""
        this.level = level
        this.size = Math.pow(2, this.level)
        for (let y = 0; y < this.size; y++) {            
            quadStr += '<div class="quad-row">'
            for (let x = 0; x < this.size; x++) {                
                this.quads[y * this.size + x] = new Quad(y * this.size + x)
                quadStr += `<div class="quad empty" title="${y * this.size + x}" id="q${y * this.size + x}"></div>`
            }
            quadStr += "</div>"
        }
        $("#quads").append(quadStr);
        $(".quad").on("click", function () {
            let index = parseInt($(this).attr("id").substr(1))
            universeManager.quads[index].changeValueManually();            
        });
    }   
    nextGeneration()
    {       
        if (this.normalisationMarker) {
            alert("Произошла нормализация");
            this.normalisationMarker = false;
        }                
        if (this.universe.next_generation()) {
            this.generation++
            if (this.generations.length <= this.generation) {
                this.generations[this.generation] = this.universe.to_string()
            }
            console.log(this.generations[this.generation].replaceAll(";", "\n"));
            this.updateQuads(this.generation - 1, this.generation)   
        }
        else {
            alert("Need to stop")
        }            
    }   
    prevGeneration(){
        if (this.generation > 0) {            
            if(this.generations[this.generation - 1].length != this.generations[this.generation].length){
                alert("Невозможно вернутся к предыдущему состоянию")
            }
            else {
                this.generation--            
                this.updateQuads(this.generation + 1,this.generation) 
            }
        }
        else {
            alert("Нельзя вернутся из изначального состояния")
        }
    }
    updateQuads(prevIndex, currIndex){        
        let prevStr = Array.from(this.generations[prevIndex].replaceAll(";",""))
        let currStr = Array.from(this.generations[currIndex].replaceAll(";",""))
        currStr.forEach((e, i) => {
            if (prevStr[i] != currStr[i]) {                                
                this.quads[i].changeValue(parseInt(currStr[i]))
            }
        })
    }    
    updateGeneration()
    {
        this.generations[this.generation] = this.getUniverseString()         
        if(this.rootLevel < this.level) {
            this.normalisationMarker = true;
        }        
        if (this.universe != null)
        {
            this.universe.delete();
        }
        this.universe = new Module.Universe(this.level, this.rootLevel, this.generations[this.generation])
        console.log(this.generations[this.generation].replaceAll(";", "\n"));
    }
    showHashList()
    {
        let list = new Module.StringList();
        this.universe.get_hash_list(list);
        alert(`Количество хэшированых квадратов - ${list.size()}`);
        list.delete();
    }        
}

class Quad {
    value = 0;
    constructor(id)
    {
        this.id = id;
    }
    changeValue(newValue) {
        this.value = newValue;
        if (this.value == 1) {            
            $("#q" + this.id.toString()).addClass("filled");
            $("#q" + this.id.toString()).removeClass("empty");
        }
        else {
            $("#q" + this.id.toString()).addClass("empty");
            $("#q" + this.id.toString()).removeClass("filled");
        }
    }
    changeValueManually() {                
        this.changeValue(this.value = this.value == 0 ? 1 : 0)
        universeManager.updateGeneration()        
    }
}


let universeManager = null;

//Функция, срабатывающая первой
$(document).ready(function(){        
   $("#life").on("module_initialized", function () {
       $("#mainMenu").toggleClass("disabled");
   });
   $("#createUniverse").click(function (e) { 
       e.preventDefault();                     
       universeManager = new UniverseManager(parseInt($("#level").val()))
       $("#createMenu").toggleClass("disabled"); 
       $("#gameMenu").toggleClass("disabled"); 
   });
   $("#nextGeneration").click(function (e) { 
       e.preventDefault();
       universeManager.nextGeneration()        
   });
   $("#previousGeneration").click(function (e) { 
       e.preventDefault();
       universeManager.prevGeneration()        
   });
   $("#showHashList").click(function (e) { 
       e.preventDefault();
       universeManager.showHashList()        
   });
   $("#stopGame").click(function (e) { 
        universeManager = null;
        $("#quads").html("");
        $("#gameMenu").toggleClass("disabled"); 
        $("#createMenu").toggleClass("disabled"); 
   });
});
//Модуль запускается после окончательной загрузки WASM
var Module = {
    onRuntimeInitialized: function() {           
      invokeModuleInitialized()              
    }    
};
function invokeModuleInitialized() {
    let module_initialized_event = new Event("module_initialized", {bubbles: true});
    life.dispatchEvent(module_initialized_event);
}