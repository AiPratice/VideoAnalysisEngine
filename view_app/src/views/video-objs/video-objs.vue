<style lang="less">
@import url("./video-objs.less");
</style>


<template>
    <div class="video_objs_main">
        <br />
        <Row :gutter="16">
            <Col span="8" v-for="video in videos" :key="video.video_id">
                <Card>
                    <p slot="title" class="video_title">
                        <Col span="18">
                            {{video.name}}
                        </Col>
                    </p>
                    <div class="video_capture">
                        <img :src="videoCaptureUrl+'?video_id='+video.video_id"></img>
                    </div>
                    
                    <p>{{video.description}}</p>
                    <p>更新时间:{{video.updated_at}}</p>
                    <Row>
                        <Col span="24">
                            <Button icon="ionic" v-if="video.object_status==0 && isLongTimeWorking==false" @click="objectDetect(video.video_id)">物体检测</Button>
                            <Button icon="ionic" v-if="isLongTimeWorking==true" disabled loading>物体检测</Button>
                            <Button icon="ionic" v-if="video.object_status==2" @click="linkToDetectResults(video.video_id)">查看物体检测结果</Button>
                        </Col>
                    </Row>
                </Card>
            </Col>
        </Row>
        <br />
    </div>
</template>

<script>
import util from "../../libs/util"
import config from "../../config"
import {eventsType} from '../../enum/event'

export default {
    name:'video_objs_index',
    mounted: function(){
        this.$store.dispatch("getVideos");
    },
    computed:{
        videos(){
            return this.$store.state.video.videos;
        },
        isLongTimeWorking(){
            return this.$store.state.isLongTimeWorking;
        }
    },
    data () {
        return {
            videoCaptureUrl:config.HOST+"/videos/capture",
            statusText:["未处理","处理中","处理完成"],
            processingVideoID: -1, //正在被处理的视频的ID
            videoForm:{
                videoID:0,
                name:'',
                description:''
            }
        }
    },
    methods: {
        objectDetect(videoID){
            let requestEvent = {
                Type:eventsType.requestObjectDetectVideo,
                Data:{
                    videoID:videoID
                }
            };
            this.$socket.sendObj(requestEvent);
            this.$Message.info('视频物体识别中，请稍候...');
            this.processingVideoID = videoID; //设置正在处理的视频id
            this.$store.commit('setIsLongTimeWorking',true);
        },
        linkToDetectResults(videoID){
            this.$router.push({
                path: 'video_objs/results/',
                name: 'video_objs_results',
                params: {
                    video_id:videoID
                }
            });
        }
    }
};
</script>
