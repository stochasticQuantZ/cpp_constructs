# Containers

Containers
Kubernetes
Openshift

2023
Cloud native
Industry changing revolution

Kubernetes -> Container orchestration

Logistics-> Containers


Container - docker 

Istio
Openshift

- management of containerized applications.
- Control plane, Nodes, Pods, Replica set, service, configmap, Ingress, Persistent vol
- Features - state management, auto scaling, self healing, multi-tenant isolation, rbac access control, 


- Digital container -> Encapsulation -> Build, ship and run applications
- Private, public cloud, isolation, efficient utilization, portability

![alt text](image-6.png)


![alt text](image-7.png)

# Container Engine: Virtualizes the OS

![alt text](image-8.png)
- any language - python java ..

![alt text](image-9.png)

![alt text](image-10.png)

![alt text](image-11.png)

![alt text](image-12.png)

![alt text](image-13.png)
![alt text](image-14.png)


- build command used with a docker file to build an image
- run with an image to run it
- docker objects: 


- n/w, storage, plugins
- FROM 
- Base image
- public repo (go, node)
- run
- cmd (one cmd instruction - only last takes effect)
- image - readonly, new layer in the image
- ![alt text](image-15.png)
- docker.io/ubuntu/18.02 --> registry, repo, tag
- ![alt text](image-16.png)
- ![alt text](image-17.png)
- ![alt text](image-18.png)
- ![alt text](image-19.png)
- ![alt text](image-20.png)

# Cmds

- docker --version
- ibmcloud version
- docker images
- docker pull hello-world
- docker container rm <container_id>
- docker ps -a
- docker build . -t myimage:v1


- 
   
    - FROM node:9.4.0-alpine
    
    - COPY app.js .

    - COPY package.json .
    
    - RUN npm install &&\
    
    - apk update &&\
    
    - apk upgrade
    
    - EXPOSE  8080
    
    - CMD node app.js
   

- 

