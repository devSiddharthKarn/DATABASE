const express = require("express");
const app = express();
const cors = require("cors");
const ejs = require("ejs");
const path = require("path");
const {exec} =  require("child_process");
const { stderr } = require("process");

//file
const demo_page = path.join(__dirname,"demo_page.ejs");

//middleware
app.use(cors());
app.set("view engine","ejs");
app.use(express.json());

//route
app.get("/",(req,res)=>{
    res.status(200).render(demo_page,{message:"Enter details"});
    console.log(`Sent the demo_page to the user`);
});


app.get("/success_page",(req,res)=>{
    const success_page = path.join(__dirname,"success_page.ejs");

    res.status(200).render(success_page,{_status:"Success",content:req.body.content});
})

app.post("/submit",(req,res)=>{
    let {filename,file_id,content}= req.body;
    

    const _exit = "\n..exitFeed";

    content=content+_exit;
    

    const process =  exec(`app.exe feed file ${filename} ${file_id}`,(error,stdout,stderr)=>{
        if(error){
            console.log("Error:"+error);
            throw error;
        }else{
            console.log(stdout);
            console.log("\n"+stderr);
        }
    })

    process.stdin.write(content);
    process.stdin.end();


    res.redirect("/success_page");
});

const PORT = 3000;
app.listen(PORT,(err)=>{
    if(err){
        console.log(`Error listening to server`);
        throw err;
    }else{
        console.log(`Listening to server at PORT ${PORT}`);
    }
})