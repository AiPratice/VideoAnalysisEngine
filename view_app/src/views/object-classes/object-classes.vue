<style lang="less">

</style>
<template>
    <div class="object-classes-main">
        <Table :columns="columns1" :data="objectClasses"></Table>
    
        <Modal 
            v-model="editModal"
            title="编辑物体分类"
            @on-ok="editOK"
        >
            <Form :label-width="80" :model="objectClassForm">
                <FormItem label="中文名称" prop="cn_name">
                    <Input v-model="objectClassForm.cnName"></Input>
                </FormItem>
            </Form>
        </Modal>    
    </div>
</template>
<script>
import util from '../../libs/util';
import config from '../../config';
export default {
    name: 'object_classes_index',
    mounted: function(){
        this.$store.dispatch('getClasses');
    },
    computed:{
        objectClasses(){
            return this.$store.state.object.classes;
        }
    },
    data() {
        return {
            editModal: false,
            objectClassForm:{
                objectClassID:0,
                cnName:'',
                enName:''
            },
            columns1:[
                {
                    title: '物体类型id',
                    key: 'object_class_id'
                },
                {
                    title: '中文名',
                    key: 'cn_name'
                },
                {
                    title: '英文名',
                    key: 'en_name'
                },
                {
                    title: '创建时间',
                    key: 'created_at'
                },
                {
                    title: '修改时间',
                    key: 'updated_at'
                },
                /*{
                    title: '操作',
                    key: 'action',
                    width: 120,
                    align: 'center',
                    render: (h, params) => {
                        return h('div', [
                            h('Button', {
                                    props: {
                                        //type: 'primary',
                                        size: 'small'
                                    },
                                    style: {
                                        marginRight: '5px'
                                    },
                                    on: {
                                        click: () => {
                                            //this.show(params.index)
                                        }
                                    }
                                }, '编辑'),
                        ])
                    }
                }*/
            ]
        };
    },
    methods: {
        editOK(){

        }
    }
}
</script>
