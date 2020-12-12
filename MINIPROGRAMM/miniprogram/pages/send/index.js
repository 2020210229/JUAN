//index.js
const Id = "655491010" // 填写在OneNet上获得的devicesId 形式就是一串数字 例子:9939133
const api_key = "HkJl7CamkwDPbphYFux5sCH9wDM=" // 填写在OneNet上的 api-key 例子: VeFI0HZ44Qn5dZO14AuLbWSlSlI=
var sandclock;
var show;

Page({
  onLoad: function () {
this.sure();
  },
  data: {
  i:1,
  "zone":'',
  "state":'',
  "target":'',
  inputHour:'',
  inputMin:'',
  sandclock:'300',
  },
  // this.setData({
  //   inputHour: e.detail.value.input_Hour,
  //   inputMin: e.detail.value.input_Min,
  //   sandclock:(inputHour-hour)*60+(inputMin-min),
  // }),
  set1:function(e){
    this.setData({
      inputHour: e.detail.value,
    })
  },

  set2:function(e){
    this.setData({
    inputMin: e.detail.value,
  })
  },

  send(e){
      // _this.sure()
  //  var _this=this;
   wx.request({
    
  
    url: `https://api.heclouds.com/cmds?device_id=${Id}`,
    method: 'POST',
    header: {
      'content-type':'application/x-www-form-urlencoded',
      'api-key': api_key 
    },
      data:`sleepTime:{${this.data.show}}`,

     success:(res)=>{
      if(this.data.i==1){
         this.data.i=0
      }
      console.log(this.data.i)
       console.log(res)
        this.setData({
         
       show:String(this.data.show),
        })
       console.log(this.data.show)  
        this.sure();
       }
     } )
    },
  sure:function(){
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
         console.log(res)
          var inputHour=this.data.inputHour;
          var inputMin=this.data.inputMin;
          var str = res.data.sysTime1;
          var hour = str.substring(8, str.length - 4);
          var min =str.substring(10, str.length - 2);
          var sandclock=(inputHour-hour)*60+(inputMin-min);
          //console.log(res,sandclock);
          if(sandclock<=0){
            sandclock=1440+sandclock;        
              this.data.show=sandclock;    
          }
          else{
            sandclock=sandclock;
              this.data.show=sandclock;
          }
          
         sandclock=String(show)
          console.log(hour);
          console.log(min);
          // console.log(show);
    }
  }) ;
 

},

})
