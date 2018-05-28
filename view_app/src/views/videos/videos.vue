<style lang="less">
@import url("./videos.less");
</style>


<template>
    <div class="videos_main">
        <Row>
            <Card>
                <Button type="primary" @click="uploadModal = true">
                    <Icon type="upload"></Icon>
                    上传
                </Button>
                &nbsp;&nbsp;
                <Button @click="mergeIndex()" v-if="isLongTimeWorking===false">
                    <Icon type="ios-film"></Icon>
                    更新合并全局索引
                </Button>
                <Button v-else disabled>
                    <Icon type="ios-film"></Icon>
                    更新合并全局索引
                </Button>
            </Card>
        </Row>
        <br />
        <Row :gutter="16">
            <Col span="8" v-for="video in videos" :key="video.video_id">
                <Card>
                    <p slot="title" class="video_title">
                        <Col span="18">
                            {{video.name}} &nbsp;
                            <span class="video_status">({{statusText[video.status]}})</span>
                        </Col>
                    </p>
                    <div class="video_capture">
                        <img :src="videoCaptureUrl+'?video_id='+video.video_id"></img>
                    </div>
                    
                    <p>{{video.Description}}</p>
                    <p>更新时间:{{video.updated_at}}</p>
                    <Row>
                        <Col span="24">
                            <Button icon="edit" @click="showVideoEditModal(video)">编辑</Button>
                            &nbsp;&nbsp;
                            <span v-if="isLongTimeWorking === false">
                                <Button icon="ios-film" v-if="video.status<2" @click="processVideo(video.video_id)">处理</Button>
                            </span>
                            <span v-else>
                                <Button icon="ios-film" v-if="video.status<2" :loading="processingVideoID===video.video_id" disabled>处理</Button>
                            </span>
                            &nbsp;&nbsp;
                            <Button icon="images" v-if="video.status>=2" @click="linkToShots(video.video_id)">视频关键帧</Button>
                        </Col>
                    </Row>
                </Card>
            </Col>
        </Row>
        <br />
        <!--<Row>
            <Page :total="100" show-elevator></Page>
        </Row>-->

        <!-- 视频上传对话框 -->
        <Modal
            v-model="uploadModal"
            title="视频上传"
            @on-ok="ok"
            @on-cancel="cancel">
            <Upload
                multiple
                type="drag"
                v-bind:action="uploadActionUrl"
                :on-success="uploadOnSuccess">
                <div style="padding: 20px 0">
                    <Icon type="ios-cloud-upload" size="52" style="color: #3399ff"></Icon>
                    <p>点击或拖动视频文件进行上传</p>
                </div>
            </Upload>
        </Modal>

        <!-- 视频信息编辑对话框 -->
        <Modal
            v-model="videoEditModal"
            title="编辑视频信息"
            @on-ok="editVideoOk">
            <Form ref="formInline" :model="videoForm">
                <FormItem prop="name" label="视频名称">
                    <Input type="text" v-model="videoForm.name" placeholder="视频名称">
                    </Input>
                </FormItem>
                <FormItem prop="description" label="视频描述">
                    <Input type="textarea" v-model="videoForm.description" placeholder="视频描述">
                    </Input>
                </FormItem>、
            </Form>
        </Modal>
    </div>
</template>

<script>
import util from "../../libs/util"
import config from "../../config"
import {eventsType} from '../../enum/event'

export default {
    name:'videos_index',
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
            uploadActionUrl: config.HOST+"/videos/save",
            uploadModal: false,
            videoEditModal:false,
            videoCaptureUrl:config.HOST+"/videos/capture",
            statusText:["未处理","处理中","处理完成"],
            processingVideoID: -1, //正在被处理的视频的ID
            videoForm:{
                video_id:0,
                name:'',
                description:''
            }
        }
    },
    methods: {
        ok () {
            //this.$Message.info('Clicked ok');
            console.log("click ok");
        },
        cancel () {
            //this.$Message.info('Clicked cancel');
            console.log("click cancel")
        },
        showVideoEditModal(video){
            this.videoForm.video_id = video.video_id;
            this.videoForm.name = video.name;
            this.videoForm.description = video.description;
            this.videoEditModal = true;
        },
        editVideoOk() {
            console.log('ok');
            util.ajax.post('/videos/edit',this.videoForm).then((response)=>{
                    console.log(response.data);
                    if(response.data.code === 200){
                        this.$store.dispatch('getVideos');
                    }else{
                        alert(response.data.message);
                    }
                }).catch((response)=>{
                    alert('编辑视频信息失败');
                });
        },
        uploadOnSuccess(){
            console.log("upload success");
            this.$store.dispatch("getVideos");
        },
        processVideo(videoID){
            let requestEvent = {
                Type:eventsType.requestProcessVideo,
                Data:{
                    videoID:videoID
                }
            };
            this.$socket.sendObj(requestEvent);
            this.$Message.info('视频处理中，请稍候...');
            this.processingVideoID = videoID; //设置正在处理的视频id
            this.$store.commit('setIsLongTimeWorking',true);
        },
        mergeIndex(){
            let requestEvent = {
                Type:eventsType.requestMergeIndex,
                Data:null
            };
            this.$socket.sendObj(requestEvent);
            this.$Message.info('索引更新合并中，请稍候...');
            this.$store.commit('setIsLongTimeWorking',true);
        },
        linkToShots(videoID){
            this.$router.push({
                path: 'shots/',
                name: 'shots_index',
                params: {
                    video_id:videoID
                }
            });
        }
    }
};
</script>
