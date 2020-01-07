const express = require("express");
const mongoose = require("mongoose");
const bodyParser = require("body-parser");
const newItem = require("./routes/api/item");
const purchase = require("./routes/api/purchase");
const login = require("./routes/api/login");
const user = require("./routes/api/user");

const app = express();

// Bodyparser middleware
app.use(bodyParser.json());

const db = require("./config/key").mongoURI;

// COnnect to the database
//"mongodb://mongo/test"
mongoose
  .connect("mongodb://mongo/test", {
    useNewUrlParser: true,
    useUnifiedTopology: true,
    useFindAndModify: false,
    useCreateIndex: true
  })
  .then(() => console.log("MongoDB connected ..."))
  .catch(err => console.log(err));

app.use("/Items", newItem);
app.use("/purchase", purchase);
app.use("/login", login);
app.use("/user", user);

const port = process.env.PORT || 3000;
app.listen(port, () => console.log(`Server started on port ${port}`));
