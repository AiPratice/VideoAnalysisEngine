<style lang="less">
@import url("./video-faces.less");
</style>


<template>
    <div class="video_faces_main">
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
                            <Button icon="ionic" v-if="video.face_status==0 && isLongTimeWorking==false" @click="faceRecognize(video.video_id)">人脸识别</Button>
                            <Button icon="ionic" v-if="video.face_status==2 && isLongTimeWorking==false" @click="faceRecognize(video.video_id)">重新识别</Button>
                            <Button icon="ionic" v-if="isLongTimeWorking==true" disabled loading>识别中</Button>
                            &nbsp;
                            <Button icon="ionic" v-if="video.face_status==2" @click="linkToRecognizeResults(video.video_id)">查看人脸识别结果</Button>
                        </Col>
                    </Row>
                </Card>
            </Col>
        </Row>
        <br />
    </div>
</template>

<script>
import util from '../../libs/util'
import config from '../../config'
import {eventsType} from '../../enum/event'

export default {
    name:'video_faces_index',
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
        faceRecognize(videoID){
            let requestEvent = {
                Type:eventsType.requestFaceRecognizeVideo,
                Data:{
                    videoID:videoID
                }
            };
            this.$socket.sendObj(requestEvent);
            this.$Message.info('视频人脸识别中，请稍候...');
            this.processingVideoID = videoID; //设置正在处理的视频id
            this.$store.commit('setIsLongTimeWorking',true);
        },
        linkToRecognizeResults(videoID){
            this.$router.push({
                path: 'video_faces/results/',
                name: 'video_faces_results',
                params: {
                    video_id:videoID
                }
            });
        }
    }
};
</script>
