## This is for the kv demo using spin 
It is a sample Pizza app where you have a few endpoints to GET, ADD and DELETE a pizza and that will be stored locally in sqlite database. 

Go to pizza directory 

```
cd pizza
```

run `spin up`

You can use `curl` commands for interacting the applications 

```
curl localhost:3000/p1 -H 'Content-Type: application/json' -d '{"saiyam":"pepperoni"}'
curl localhost:3000/p2 -H 'Content-Type: application/json' -d '{"Rishit":"Margherita"}'
curl localhost:3000/p2                   
{"Rishit":"Margherita"}%                                                           curl localhost:3000/p1 
{"saiyam":"pepperoni"}%                                                            curl -X DELETE localhost:3000/p2         
curl localhost:3000/p2   

```
