# This package is required for Accessing APIS (HTTP or HTTPS URLS from Web)
library(httr)
#This package exposes some additional functions to convert json/text to data frame
library(rlist)
#This package exposes some additional functions to convert json/text to data frame
library(jsonlite)
#This library is used to manipulate data
library(dplyr)
#this is for plotting
library(ggplot2)
#this is for "melting" data
library(reshape2)

theme_ts_space<-theme_grey() +
		theme(
#		panel.grid.major = element_blank(),
#		panel.grid.minor = element_blank(),
		panel.background = element_rect(fill="white", colour="black", size=2),
#		legend.key       = element_blank(),
#		legend.text      = element_text(size=20.5),
#		legend.text      = element_blank(),
#		legend.title     = element_text(size=20.5),
		axis.text.x = element_text(size=10,colour="black",hjust=1,angle=45),
		axis.text.y = element_text(size=10,colour="black",vjust=.3),
		axis.title.x = element_text(size=10),
		axis.title.y = element_text(size=10,vjust=-1),
#		plot.title = element_text(hjust = 0.5,size=10,face="bold"),		
#		legend.position  = "left",
#		legend.position  = "none",
		plot.margin = unit(c(1,1,1,1), "cm"),
		panel.border = element_rect(colour = "black", fill=NA, size=2)
		)


resp<-GET("https://thingspeak.com/channels/1030130/feeds.json?results=8000&start=2020-04-03%2003:00:00&timezone=America%2FNew_York")
#.When we get the response from API we will use to very basic methods of httr.
http_type(resp)  #. This method will tell us what is the type of response fetched from GET() call to the API.

jsonRespText<-content(resp,as="text") 
#jsonRespText

jsonRespParsed<-content(resp,as="parsed") 
#jsonRespParsed


modJson<-jsonRespParsed$feeds #. Access data element of whole list and ignore other vectors
#modJson

#Using dplyr and base R
modJson%>%bind_rows%>%select(created_at,field1,field2,field3,field4,field5,field6,field7,field8)


HosenArgon<-as.data.frame(modJson%>%bind_rows%>%select(created_at,field1,field2,field3,field4,field5,field6,field7,field8))
names(HosenArgon)<-c("DateTime","Temperature","Humidity","Pressure","Voltage","AQSlope","AQVoltage","LPO","DustPercent")
HosenArgon$DateTime<-as.POSIXct(gsub("-04:00","",HosenArgon$DateTime,fixed=TRUE),format="%Y-%m-%dT%H:%M:%S")

df_clean <- HosenArgon %>% mutate_if(is.factor, as.character)  %>% mutate_if(is.character, as.numeric)

melt4plots<-melt(df_clean,id.vars=c("DateTime"))

ggplot(melt4plots,aes(DateTime,value))+
theme_ts_space+
theme(
strip.background = element_blank(),
strip.placement="outside")+
facet_wrap(~variable,scales="free_y",strip.position="left")+
geom_point(size=2)+
scale_x_datetime(limits=c(as.POSIXct("2020-04-03 03:29:00",tz="GMT+4"),as.POSIXct("2020-04-03 14:46:28",tz="GMT+4")))+
ylab("")





