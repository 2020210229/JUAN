// miniprogram/pages/check/check.js
var showMonth,startMonth,startDay;
var showDay;

var day1,day2,day3,day4,day5,day6,day7;
var str1,str2,str3,str4,str5,str6,str7;
var value1,value2,value3,value4,value5,value6,value7;
var value1end,value2end,value3end,value4end,value5end,value6end,value7end;
var value1judge,value2judge,value3judge,value4judge,value5judge,value6judge,value7judge;
var valueMinSum,valueSum,valueSencondEnd,valueSecondSum;
Page({
  sureDate:function(){
    wx.request({
      url:'https://quan.suning.com/getSysTime.do',
      method:'POST',
      header:{
        "key":"oursparkspace",
      },
      data:{
       "zone":'+8',//设置时区为+8（东八区）
             },
       success:(res)=>{
        var _this=this;
       // console.log(res)
          var str = res.data.sysTime2;
          var month = str.substring(5, str.length - 12);
          var day =str.substring(8, str.length-9 );
          var dayOfThatMonth;var startMonth,startDay;
          
          day=Number(day);
          startDay=day-7;
          if(startDay<=0){
            startMonth=month-1
            if(startMonth==1||startMonth==3||startMonth==5||startMonth==7||startMonth==8|startMonth==10||startMonth==12){
              dayOfThatMonth=31;
            }
            else if(startMonth==4||startMonth==6||startMonth==9||startMonth==11){
              dayOfThatMonth=30;
            }
            else {
              dayOfThatMonth=28;
            }
            startDay=dayOfThatMonth+startDay;
          }
          else{
            startMonth=month
            if(startMonth==1||startMonth==3||startMonth==5||startMonth==7||startMonth==8|startMonth==10||startMonth==12){
              dayOfThatMonth=31;
            }
            else if(startMonth==4||startMonth==6||startMonth==9||startMonth==11){
              dayOfThatMonth=30;
            }
            else {
              dayOfThatMonth=28;
            }
          }
          
          showMonth=String(startMonth);
          showDay=String(startDay);
          console.log(showMonth,showDay);
          wx.request({
          // url: `http://api.heclouds.com/devices/642668048/datapoints?datastream_id=setTime&start=2020-${showMonth}-${showDay}T00:00:00&limit=1000`,
          url: `https://api.heclouds.com/devices/655491010/datapoints?datastream_id=sitTime&limit=7`,
            method:'GET',
            header:{
               'api-key':'HkJl7CamkwDPbphYFux5sCH9wDM='
              // 'api-key':'ZxZbgsskGLYuAjVvJ5WTQRuqDbQ='
            },
            
            data:{     

            },
 
            success:(res)=>{   
              console.log(res)   
              console.log(res.data.data.datastreams[0].datapoints[0].at,res.data.data.datastreams[0].datapoints[0].value),
              console.log(res.data.data.datastreams[0].datapoints[1].at,res.data.data.datastreams[0].datapoints[1].value),
              console.log(res.data.data.datastreams[0].datapoints[2].at,res.data.data.datastreams[0].datapoints[2].value),
              console.log(res.data.data.datastreams[0].datapoints[3].at,res.data.data.datastreams[0].datapoints[3].value),
              console.log(res.data.data.datastreams[0].datapoints[4].at,res.data.data.datastreams[0].datapoints[4].value),
              console.log(res.data.data.datastreams[0].datapoints[5].at,res.data.data.datastreams[0].datapoints[5].value),
              console.log(res.data.data.datastreams[0].datapoints[6].at,res.data.data.datastreams[0].datapoints[6].value)
                str1=res.data.data.datastreams[0].datapoints[1].at
                day1=str1.substring(0,str1.length-13)
                value1=res.data.data.datastreams[0].datapoints[1].value*10
                value1judge=(value1)%60
                if(value1judge<30){
                value1end=(value1-(value1)%60)/60
                }
                else{
                value1end=((value1-(value1)%60)/60)+1
                }
                console.log(value1end)

                str2=res.data.data.datastreams[0].datapoints[2].at
                day2=str2.substring(0,str2.length-13)
                value2=res.data.data.datastreams[0].datapoints[2].value*10
                value2judge=(value2)%60
                if(value2judge<30){
                value2end=(value2-(value2)%60)/60
                }
                else{
                value2end=((value2-(value2)%60)/60)+1
                }
                console.log(value2end)

                str3=res.data.data.datastreams[0].datapoints[3].at
                day3=str3.substring(0,str3.length-13)
                value3=res.data.data.datastreams[0].datapoints[3].value*10
                value3judge=(value3)%60
                if(value3judge<30){
                value3end=(value3-(value3)%60)/60
                }
                else{
                value3end=((value3-(value3)%60)/60)+1
                }
                console.log(value3end)

                str4=res.data.data.datastreams[0].datapoints[4].at
                day4=str4.substring(0,str4.length-13)
                value4=res.data.data.datastreams[0].datapoints[4].value*10
                value4judge=(value4)%60
                if(value4judge<30){
                value4end=(value4-(value4)%60)/60
                }
                else{
                value4end=((value4-(value4)%60)/60)+1
                }
                console.log(value4end)

                str5=res.data.data.datastreams[0].datapoints[5].at
                day5=str5.substring(0,str5.length-13)
                value5=res.data.data.datastreams[0].datapoints[5].value*10
                value5judge=(value5)%60
                if(value5judge<30){
                value5end=(value5-(value5)%60)/60
                }
                else{
                value5end=((value5-(value5)%60)/60)+1
                }
                console.log(value5end)

                str6=res.data.data.datastreams[0].datapoints[6].at
                day6=str6.substring(0,str6.length-13)
                value6=res.data.data.datastreams[0].datapoints[6].value*10
                value6judge=(value6)%60
                if(value6judge<30){
                value6end=(value6-(value6)%60)/60
                }
                else{
                value6end=((value6-(value6)%60)/60)+1
                }
                console.log(value6end)

                str7=res.data.data.datastreams[0].datapoints[0].at
                day7=str7.substring(0,str7.length-13)
                value7=res.data.data.datastreams[0].datapoints[0].value*10
                value7judge=(value7)%60
                if(value7judge<30){
                value7end=(value7-(value7)%60)/60
                }
                else{
                value7end=((value7-(value7)%60)/60)+1
                }
                console.log(value7end)

                valueMinSum=Number(value1end)+Number(value2end)+Number(value3end)+Number(value4end)+Number(value5end)+Number(value6end)+Number(value7end)
                
                this.setData({
                  day1:day1,
                  value1:value1end,

                  day2:day2,
                  value2:value2end,

                  day3:day3,
                  value3:value3end,

                  day4:day4,
                  value4:value4end,

                  day5:day5,
                  value5:value5end,

                  day6:day6,
                  value6:value6end,

                  day7:day7,
                  value7:value7end,
                       
                 
                  valueMinSum:valueMinSum,

                })
             },            
          });
          
          
    //       console.log(month);
     //    console.log(day[5]);
    //       console.log(startMonth);
    //       console.log(startDay);
     }
  })
},
  history:function(){[
    this.sureDate(),
    this.setData({
     showMonth:String(startMonth),
     showDay:String(startDay),
    }),   
    console.log(startDay,startMonth),
    wx.request({
      url: "http://api.heclouds.com/devices/655491010/datapoints?datastream_id=sitTime&start=2020-12-02T00:00:00&limit=4",
      method:'GET',
      header:{
        'api-key':'HkJl7CamkwDPbphYFux5sCH9wDM='
      },
      data:{       
      },
      success:function(res){[
        console.log(res),
       ] }
    })
   ]}
})