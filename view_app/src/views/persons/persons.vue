<style lang="less">
@import url("./persons.less");
</style>

<template>
    <div class="persons_main">
        <Row>
            <Card>
                <Button @click="addPersonModal = true">
                    <Icon type="person-add"></Icon>
                    添加标注人物
                </Button>
                &nbsp;&nbsp;
                <span v-if="isLongTimeWorking === false">
                    <Button @click="trainFaceClassifier()">
                        <Icon type="filing"></Icon>
                        训练人脸分类器
                    </Button>
                </span>
                <span v-else>
                    <Button :loading="true" disabled>
                        <Icon type="filing"></Icon>
                        训练人脸分类器
                    </Button>
                </span>
            </Card>
        </Row>
        <br/>
        <Row :gutter="16">
            <Col span="8" v-for="person in persons" :key="person.PersonID">
                <Card>
                    <p slot="title" class="person_title">
                        {{person.name}}
                    </p>
                    <p v-if="person.Description == ''">
                        暂无描述信息
                    </p>
                    <p v-else>{{person.Description}}</p>
                    <br/>
                    <Row>
                        <Col span="24">
                            <Button icon="edit" @click="editPerson(person)">编辑信息</Button>
                            &nbsp;
                            <Button icon="android-image" @click="editPersonFaces(person)">人脸数据</Button>
                            &nbsp;
                            <Button icon="android-delete">删除</Button>
                        </Col>
                    </Row>
                </Card>
            </Col>
        </Row>
        
        <!-- 标注人物添加对话框 -->
        <Modal 
            v-model="addPersonModal"
            title="添加标注人物"
            @on-ok="addPersonOk" 
            @on-cancel="addPersonCancel">
            <Form :label-width="80">
                <FormItem label="姓名" prop="name">
                    <Input v-model="name"></Input>
                </FormItem>
                <FormItem label="描述" prop="description">
                    <Input v-model="description"></Input>
                </FormItem>
            </Form>
        </Modal>

        <!-- 人脸数据对话框 -->
        <Modal
            v-model="editPersonFacesModal"
            :title="editPersonFacesModalTitle"
            width="800"
            >
            <div class="person_faces_container">
                <Row :gutter="8">
                    <Col span="8" v-for="personFace in personFaces" :key="personFace.person_face_id">
                        <Card>
                            <p slot="title">
                                person_face_id:{{personFace.person_face_id}}
                            </p>
                            <a href="#" slot="extra" @click.prevent="deletePersonFace(personFace.person_face_id)">
                                <Icon type="android-delete"></Icon>
                                删除
                            </a>
                            <div class="face_img">
                                <img :src="personFaceImgUrl+'?person_face_id='+personFace.person_face_id" class="face_img"></img>
                            </div>

                        </Card>
                    </Col>
                </Row>
            </div>
            <div slot="footer">
                <Upload 
                    v-bind:action="uploadActionUrl" 
                    v-bind:data="personData"
                    :on-success="uploadPersonFaceSucceed"
                    :show-upload-list="false">
                    <Button icon="android-upload">上传</Button>
                </Upload>
            </div>
        </Modal>

        <!-- 标注人物编辑对话框 -->
        <Modal 
            v-model="editPersonModal"
            title="编辑标注人物"
            @on-ok="editPersonOk">
            <Form :label-width="80">
                <FormItem label="姓名" prop="name">
                    <Input v-model="name"></Input>
                </FormItem>
                <FormItem label="描述" prop="description">
                    <Input v-model="description"></Input>
                </FormItem>
            </Form>
        </Modal>
    </div>
</template>

<script>
import util from '../../libs/util';
import config from '../../config';
import {eventsType} from '../../enum/event';

export default {
    name:'persons_index',
    mounted(){
        this.$store.dispatch('getPersons');
    },
    computed:{
        persons(){
            return this.$store.state.person.persons;
        },
        isLongTimeWorking(){
            return this.$store.state.isLongTimeWorking;
        }
    },
    data(){
        return {
            addPersonModal:false,
            editPersonModal:false,
            editPersonFacesModal:false,
            editPersonFacesModalTitle:'',
            editPersonID:0,
            name:'',
            description:'',
            uploadActionUrl:config.HOST+'/personfaces/save',
            personData:null,
            personFaces:[],
            personFaceImgUrl:config.HOST + '/personfaces/face'
        }
    },
    methods:{
        addPersonOk() {
            let person = {
                name:this.name,
                description:this.description
            };
            util.ajax.post('/persons/add',person)
                .then((response)=>{
                    console.log(response.data);
                    if(response.data.code === 200){
                        this.$store.dispatch('getPersons');
                    }else{
                        alert(response.data.message);
                    }
                }).catch((response)=>{
                    alert('添加预标注人物失败');
                });
        },
        editPersonOk() {
            let person = {
                PersonID:this.editPersonID,
                Name:this.name,
                Description:this.description
            };
            util.ajax.post('/persons/edit',person)
                .then((response)=>{
                    let data = response.data;
                    if(data.code === 200){
                        this.$store.dispatch('getPersons');
                    }else{
                        alert(data.message);
                    }
                }).catch((response)=>{
                    alert('编辑预标注人物失败');
                });
        },
        addPersonCancel() {

        },
        editPerson(person) {
            this.editPersonID = person.person_id;
            this.editPersonModal = true;
            this.name = person.name;
            this.description = person.description;
        },
        editPersonFaces(person) {
            this.editPersonFacesModal = true;
            this.editPersonID = person.person_id;
            this.editPersonFacesModalTitle = person.name + ' —— 人脸数据管理';
            this.personData = {
                person_id:person.person_id
            }
            this.updatePersonFaces(person.person_id);
        },
        updatePersonFaces(person_id) {
            util.ajax.get('/personfaces/list?person_id='+person_id)
            .then((response)=>{
                this.personFaces = response.data;
            }).catch((error)=>{
                alert('获取人脸图片失败');
                console.log(error);
            });
        },
        uploadPersonFaceSucceed(){
            this.updatePersonFaces(this.editPersonID);
        },
        deletePersonFace(personFaceID){
            let ret = confirm('确定要删除吗');
            if(ret){
                util.ajax.post('/personfaces/delete',{
                    personFaceID:personFaceID
                }).then((response)=>{
                    let data = response.data;
                    if(data.code === 200){
                        this.updatePersonFaces(this.editPersonID);
                    }else{
                        alert(data.message);
                    }
                }).catch((error)=>{
                    console.log(error);
                    alert('删除人脸图片失败');
                });
            }
        },
        trainFaceClassifier(){
            let requestEvent = {
                Type:eventsType.requestTrainFaceClassifier
            };
            this.$socket.sendObj(requestEvent);
            this.$store.commit('setIsLongTimeWorking',true);
        }
    }
}
</script>

