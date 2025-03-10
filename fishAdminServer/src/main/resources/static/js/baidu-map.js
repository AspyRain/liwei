// 异步加载地图
function loadBaiduMap() {
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = '//api.map.baidu.com/api?type=webgl&v=1.0&ak=OeMGqve2XKvbyczH8bNZ8GuDLlZu7oxl&callback=initBaiduMap';
    document.body.appendChild(script);
  }
  
  // 初始化地图
  function initBaiduMap() {
    var map = new BMapGL.Map('map'); // 创建Map实例
    var point = new BMapGL.Point(106.55, 29.57); // 创建点坐标
    map.centerAndZoom(point, 8); // 初始化地图,设置中心点坐标和地图级别
    map.enableScrollWheelZoom(true); // 开启鼠标滚轮缩放
    map.setMapStyleV2({
      styleJson:[
        {
          "featureType": "country",
          "elementType": "labels",
          "stylers": {
            "visibility": "off"
          }
        }, 
        {
          "featureType": "districtlabel",
          "elementType": "labels.text",
          "stylers": {
            "fontsize": 27
          }
        }, 
        {
          "featureType": "city",
          "elementType": "labels.text",
          "stylers": {
            "fontsize": 26
          }
        }
      ]
    });
    return true;
  }
  

  