/**
 * 
 * Manipulating the DOM exercise.
 * Exercise programmatically builds navigation,
 * scrolls to anchors from navigation,
 * and highlights section in viewport upon scrolling.
 * 
 * Dependencies: None
 * 
 * JS Version: ES2015/ES6
 * 
 * JS Standard: ESlint
 * 
*/

/**
 * Define Global Variables
 * 
*/
class Section  {
    constructor(name ,section_elem){
        this.name = name;
       
        this.section_elem = section_elem;
    }
    setActiveStatus(status){
        this.status=status;
    }
    getName(){
        return this.name;
    }
   
    getStatus(){
        return this.status;
    }
    getSectionElem(){
        return this.section_elem;
    }
    setListItem(elem) {
        this.list_item = elem;
    }
    getListItem(){
        return this.list_item;
    }
}

let sections =  document.querySelectorAll("section");
const nav_list = document.getElementById("navbar__list");
let header = document.querySelector("header");
/**
 * End Global Variables
 * Start Helper Functions
 * 
*/

function constructSections(sections){
    let section_objs = [];
    for(const sec of sections){
        const name = sec.dataset["nav"];
        let section = new Section(name,sec);
        if(sec.classList.contains("your-active-class"))
            section.setActiveStatus(true);
        else
            section.setActiveStatus(false);
        section_objs.push(section);
        
    }
    
    return section_objs;
}



/*
Click listener function for nav bar section element
when clicked get the name of clicked element and get the corresponding section element and scroll to it
*/
function scrollTo(evt){
    const target_name=  evt.target.textContent;
    for(const sec of section_objs){
        if(sec.getName()==target_name)
             sec.getSectionElem().scrollIntoView({behavior: 'smooth'
            });
    }
 }

//hide nav bar by moving header element to top
function hideNavBar() {
    header.style.top="-50px";
}
//show nav bar by setting header top to 0px
function viewNavBar() {
    header.style.top = "0px";

}


//Listener function for scroll event
// gets the current section in viewport
// if already active do nothing
// if not active sets the active section inactive and activate the section in viewport
function setSectionActive(){
    for(const sec of section_objs){
        if(isElementInViewport(sec.getSectionElem()) && sec.getStatus() === true){
            return;
        }
        if(isElementInViewport(sec.getSectionElem())){
            let active_section = getActiveSection(section_objs);
            setInactive(active_section);
            setActive(sec);
            return;
        }
    }
}
//sets a section inactive 
function setInactive(section_obj) {
            section_obj.getSectionElem().classList.remove("your-active-class");
            section_obj.setActiveStatus(false);
            section_obj.getListItem().classList.remove("active__nav");
}

//sets an section active
function setActive(section_obj) {
    section_obj.getSectionElem().classList.add("your-active-class");
    section_obj.setActiveStatus(true);
    section_obj.getListItem().classList.add("active__nav")
}

//returns the active section from section_objects 
function getActiveSection(section_objs){
    for(let sec of section_objs){
        if(sec.getStatus() === true){
            return sec;
        }
    }
}
function isElementInViewport (el) {

  
    let rect     = el.getBoundingClientRect(),
    vWidth   = window.innerWidth || document.documentElement.clientWidth,
    vHeight  = window.innerHeight || document.documentElement.clientHeight,
    efp      = function (x, y) { return document.elementFromPoint(x, y) };     

// Return false if it's not in the viewport
if (rect.right < 0 || rect.bottom < 0 
        || rect.left > vWidth || rect.top > vHeight)
    return false;

//Calculate mid point of element and return true if it's visible;
const y_mid = (rect.top+rect.bottom)/2;
const x_mid = (rect.right+rect.left)/2;
return el.contains(efp(x_mid,y_mid));

}

const section_objs = constructSections(sections);
/**
 * End Helper Functions
 * Begin Main Functions
 * 
*/
// build the nav

(function buildNav(section_objs,nav_list){
    nav_list.style.display = "none";
    section_objs.forEach(sec => {
        const list_item = document.createElement('li');
        list_item.textContent = sec.getName();
        list_item.classList.add("menu__link");
        if(sec.getStatus())
            list_item.classList.add("active__nav");
        
        nav_list.appendChild(list_item);
        sec.setListItem(list_item);
        list_item.addEventListener("click",scrollTo);
    
    });
        
    nav_list.style.display = "block";
    
})(section_objs,nav_list);

// Add class 'active' to section when near top of viewport
setSectionActive(section_objs);
// Scroll to anchor ID using scrollTO event


/**
 * End Main Functions
 * Begin Events
 * 
*/

// Build menu 

// Scroll to section on link click

// Set sections as active

document.addEventListener("scroll",setSectionActive);
let timer;
window.addEventListener("scroll",() => {
    if(timer != "undefined")
    window.clearTimeout(timer);
   
  if (document.body.scrollTop > 20 || document.documentElement.scrollTop > 20) {
    viewNavBar();
   } else {
     hideNavBar();
   }
   timer = setTimeout(hideNavBar,5000);
   
},false)
