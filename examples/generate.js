let nEvents = 30;
let nRooms = 5;
let nStudents = 200;
let nFeatures = 2;

let p = console.log;

p(nEvents + " " + nRooms + " " + nFeatures + " " + nStudents);

for (let i = 0; i < nRooms; i++) {
    p(Math.floor(Math.random() * 20)+200);
}

for(let i = 0; i<nStudents;i++){
    for (let j = 0; j < nEvents; j++) {
        p(Math.round(Math.random()));
        
    }
}

for(let i = 0; i<nRooms;i++){
    for (let j = 0; j < nFeatures; j++) {
        p(Math.round(Math.random()));
        
    }
}

for(let i = 0; i<nEvents;i++){
    for (let j = 0; j < nFeatures; j++) {
        p(Math.round(Math.random()));
        
    }
}