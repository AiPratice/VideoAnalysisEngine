<style lang="less">
    .shots_main{
        .second_label{
            text-align: center;
        }
    }
</style>
<template>
    <div class="shots_main">
        <Row>
            <h2>视频关键帧:{{videoName}}</h2>
            <br/>
        </Row>
        <Row>
            <Col span="4" v-for="shot in shots" :key="shot.frame_id">
                <Card>
                    <img :src="frameImgUrlPrefix+shot.frame_id" width="100%"></img>
                    <p class="second_label">{{shot.second}}</p>
                </Card>
            </Col>
        </Row>
    </div>
</template>
<script>
import config from '../../config';
import util from '../../libs/util';
export default {
    name:'shots_index',
    activated(){
        let tempVideoID = parseInt(this.$route.params.video_id);
        if(tempVideoID == this.videoID){
            return;
        }
        this.videoID = tempVideoID;
        util.ajax.get('/videos/find?video_id='+this.videoID)
            .then((response)=>{
                this.videoName = response.data.name;
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取视频');
            });
        util.ajax.get('/frames/shots?video_id='+this.videoID)
            .then((response)=>{
                this.shots = response.data;
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取关键帧列表');
            });
        console.log(this.videoID);
        console.log(this.videoName);
    },
    computed:{

    },
    data(){
        return{
            videoId:0,//视频id
            videoName:'',//
            shots:[],
            frameImgUrlPrefix:config.HOST+'/frames/frame_img?frame_id='
        };
    },
    methods:{

    }
}
</script>
