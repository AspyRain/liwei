Vue.component('flaskpolyline', {
    template:`
        <bm-polyline :path="path" :stroke-color="color" :stroke-opacity="0.8" :stroke-weight="10" :icons="/map_demo/imgs/airplane.png"></bm-polyline>
    `,
    props: {
        path: {
            type: Object,
            default: function () {
                return []
            },
        },
        color:{
            type:String,
            default:"blue"
        },
        iconsarray:{
            type:Array
        },
    },
});