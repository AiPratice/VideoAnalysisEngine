<style lang="less">
@import url("./search.less");
</style>
<template>
    <div class="search_main">
        <Row :gutter="16">
            <Col span="12">
                <Card>
                    <p slot="title">
                        <Icon type="search"></Icon>
                        以图搜索视频
                    </p>
                    <Upload 
                        name="file"
                        type="drag" 
                        v-bind:action="uploadActionUrl" 
                        :format="['jpg','jpeg','png','bmp']"
                        :max-size="5120"
                        :before-upload="beforeUpload"
                        :on-exceeded-size="handleMaxSize"
                        :on-success="uploadOnSuccess"
                        :on-progress="handleUploadProgress"
                        :show-upload-list="false"
                        v-if="!isProcessing"
                        >
                        <div class="upload_container_box" style="border:none">
                            <Icon type="ios-cloud-upload" size="65" style="color: #3399ff"></Icon>
                            <p>单击或拖动图像进行文件上传</p>
                        </div>
                    </Upload>
                    <div class="upload_container_box" v-else>
                        <Icon type="loop" size="65" style="color: #3399ff"></Icon>
                        <p>正在对图像进行处理，请稍后...</p>
                    </div>
                </Card>
                
            </Col>
            <Col span="12">
                <Card>
                    <p slot="title">
                        <Icon type="image"></Icon>
                        图片内容
                    </p>
                    <div class="upload_container_box">
                        <img :src="previewImgSrc" height="100%" v-if="previewImgSrc.length>0"/>
                    </div>
                </Card>
            </Col>
        </Row>
        <br/>
        <Row :gutter="16">
            <Col span="24" v-if="results.length > 0">
                <p>搜索结果(共:{{results.length}}个)：</p>
            </Col>
            
            <Col span="4" v-for="result in results" :key="result.frame_id">
                <Card>
                    <div>
                        <p slot="title">
                            <Icon type="ios-film-outline"></Icon>
                            {{result.frame_id}}
                        </p>
                        <img v-bind:src="result.src" height="60px" class="search_result_item"/>
                        <Button type="primary" icon="film-marker" size="small" @click="playVideo(result.video_id,result.second)">查看视频</Button>
                    </div>
                </Card>
                <br/>
            </Col>
        </Row>

        <!-- 视频播放对话框 -->
        <Modal
            v-model="playerModal"
            title="视频播放"
            @on-ok="playerModalOk"
            @on-cancel="playerModalCancel">
            <video id="searchVideoPlayer" :src="videoUrl+playvideoID" controls="controls" v-if="playerModal" width="100%">
                无法播放视频
            </video>
        </Modal>
    </div>
</template>
<script>
import config from "../../config"
import util from "../../libs/util"

export default {
    name:'search_index',
    mounted(){
        
    },
    data() {
        return {
            videoUrl:config.HOST+'/videos/video?video_id=',
            uploadActionUrl: config.HOST+"/videos/search",
            results:[],//搜索结果图像
            previewImgSrc:'',
            isProcessing:false,
            playerModal:false, //播放对话框
            playVideoID:0, //被播放的视频id
            playSecond:0 //播放视频的定位秒数
        }
    },
    methods:{
        beforeUpload(file){
            if(file){
                this.isProcessing = true;
                let reader = new FileReader();
                reader.readAsDataURL(file);
                reader.addEventListener('load',()=>{
                    this.previewImgSrc = reader.result;
                });
            }
        },
        uploadOnSuccess(resp,file){
            this.results = [];
            console.log(resp);
            for(let i in resp){
                this.results.push({
                    frame_id:resp[i].frame_id,
                    video_id:resp[i].video_id,
                    src:config.HOST+"/frames/frame_img?frame_id="+resp[i].frame_id,
                    score:resp[i].score,
                    second:resp[i].second
                });
            }
            this.isProcessing = false;
        },
        handleMaxSize(){

        },
        handleUploadProgress(event, file, fileList){
            console.log(event);
            console.log(file);
            console.log(fileList);
        },
        playVideo(video_id,second){
            console.log(video_id);
            console.log(second);
            //视频播放
            this.playvideoID = video_id;
            this.playSecond = second;
            this.playerModal = true;

            setTimeout(()=>{
                let searchVideoPlayer = document.getElementById('searchVideoPlayer');
                searchVideoPlayer.currentTime = second;
            },300);
        },
        playerModalOk(){
            this.playerModal = false;
        },
        playerModalCancel(){
            this.playerModal = false;
        }
    }
};
</script>

